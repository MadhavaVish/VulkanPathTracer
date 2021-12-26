#pragma once

#include "VPTWindow.hpp"
#include "VPTDevice.hpp"
#include "VPTSwapChain.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace VPT {

	class VPTRenderer {
	public:

		VPTRenderer(VPTWindow &window, VPTDevice &device);
		~VPTRenderer();
		
		VPTRenderer(const VPTRenderer&) = delete;
		VPTRenderer &operator=(const VPTRenderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const {return vptSwapChain->getRenderPass();}
		float getAspectRatio() const { return vptSwapChain->extentAspectRatio();}
		uint32_t getImageCount() const { return static_cast<uint32_t>(vptSwapChain->imageCount()); }
        bool isFrameInProgress() const {return isFrameStarted;}

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame isn't in progress");
            return commandBuffers[currentFrameIndex];
        }

		int getFrameIndex() const 
		{
			assert(isFrameStarted && "Cannot get frame index when frame isn't in progress");
			return currentFrameIndex;
		}

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();


		VPTWindow& vptWindow;
		VPTDevice& vptDevice;
		std::unique_ptr<VPTSwapChain> vptSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

        
        uint32_t currentImageIndex;
		int currentFrameIndex{0};
        bool isFrameStarted{false};
	};

}