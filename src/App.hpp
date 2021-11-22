#pragma once

#include "VPTWindow.hpp"
#include "VPTPipeline.hpp"
#include "VPTDevice.hpp"
#include "VPTSwapChain.hpp"
#include "VPTModel.hpp"

#include <memory>
#include <vector>

namespace VPT {

	class App {
	public:

		App();
		~App();
		
		App(const App&) = delete;
		App &operator=(const App&) = delete;
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		

		VPTWindow vptWindow{ WIDTH,HEIGHT, "YO waddup" };
		VPTDevice vptDevice{vptWindow};
		std::unique_ptr<VPTSwapChain> vptSwapChain;

		std::unique_ptr<VPTPipeline> vptPipeline;
		VkPipelineLayout vptPipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<VPTModel> vptModel;
	};

}