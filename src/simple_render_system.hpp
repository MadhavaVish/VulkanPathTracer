#pragma once

#include "VPTPipeline.hpp"
#include "VPTDevice.hpp"
#include "VPTSceneObject.hpp"
#include "VPTCamera.hpp"

#include <memory>
#include <vector>

namespace VPT
{

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(VPTDevice &device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem &) = delete;
		SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

		void renderSceneObjects(VkCommandBuffer commandBuffer, std::vector<VPTSceneObject> &sceneObjects, const VPTCamera &camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		VPTDevice &vptDevice;
		std::unique_ptr<VPTPipeline> vptPipeline;
		VkPipelineLayout vptPipelineLayout;
	};

}