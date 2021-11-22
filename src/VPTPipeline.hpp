#pragma once

#include "VPTDevice.hpp"

#include <string>
#include <vector>
namespace VPT
{

	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo &) = delete;
		PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class VPTPipeline
	{
	public:
		VPTPipeline(
			VPTDevice &device,
			const std::string &vertFilePath,
			const std::string &fragFilePath,
			const PipelineConfigInfo &configInfo);
		~VPTPipeline();
		 
		VPTPipeline(const VPTPipeline &) = delete;
		VPTPipeline operator=(const VPTPipeline &) = delete;

		void bind(VkCommandBuffer commandBuffer);

		static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

	private:
		static std::vector<char> readFile(const std::string &filePath);

		void createGraphicsPipeline(
			const std::string &vertFilePath,
			const std::string &fragFilePath,
			const PipelineConfigInfo &configInfo);

		void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

		VPTDevice &vptDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}