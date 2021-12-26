#include "VPTRenderer.hpp"

#include <stdexcept>
#include <array>
#include <cassert>

namespace VPT
{
	VPTRenderer::VPTRenderer(VPTWindow &window, VPTDevice &device) : vptWindow{window}, vptDevice{device}
	{
		recreateSwapChain();
		createCommandBuffers();
	}
	VPTRenderer::~VPTRenderer()
	{
		freeCommandBuffers();
	}

	void VPTRenderer::recreateSwapChain()
	{
		auto extent = vptWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = vptWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(vptDevice.device());
		if (vptSwapChain == nullptr)
		{
			vptSwapChain = std::make_unique<VPTSwapChain>(vptDevice, extent);
		}
		else
		{
			std::shared_ptr<VPTSwapChain> oldSwapChain = std::move(vptSwapChain);
			vptSwapChain = std::make_unique<VPTSwapChain>(vptDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*vptSwapChain.get()))
			{
				throw std::runtime_error("Swap chain image or depth format has changed!");
			}
		}

		// come back
	}
	void VPTRenderer::createCommandBuffers()
	{
		commandBuffers.resize(VPTSwapChain::MAX_FRAMES_IN_FLIGHT);
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vptDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(vptDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers");
		}
	}
	void VPTRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			vptDevice.device(),
			vptDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer VPTRenderer::beginFrame()
	{
		assert(!isFrameStarted && "Can't call beginFrame while already in progress");

		auto result = vptSwapChain->acquireNextImage(&currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		return commandBuffer;
	}
	void VPTRenderer::endFrame()
	{
		assert(isFrameStarted && "Can't call endFrame when frame isn't in progress");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer");
		}
		auto result = vptSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vptWindow.wasWindowResized())
		{
			vptWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image");
		}
		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % VPTSwapChain::MAX_FRAMES_IN_FLIGHT;
	}
	void VPTRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin a render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vptSwapChain->getRenderPass();
		renderPassInfo.framebuffer = vptSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = vptSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(vptSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(vptSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{{0, 0}, vptSwapChain->getSwapChainExtent()};
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}
	void VPTRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't end a render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}
}