#pragma once

#include "VPTWindow.hpp"
#include "VPTPipeline.hpp"

namespace VPT {

	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private: 
		VPTWindow vptWindow{ WIDTH,HEIGHT, "YO waddup" };
		VPTPipeline vptPipeline{"shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv"};
			
	};

}