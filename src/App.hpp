#pragma once

#include "VPTWindow.hpp"
#include "VPTDevice.hpp"
#include "VPTRenderer.hpp"
#include "VPTSceneObject.hpp"
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
		void loadSceneObjects();
		

		VPTWindow vptWindow{ WIDTH,HEIGHT, "YO waddup" };
		VPTDevice vptDevice{vptWindow};
		VPTRenderer vptRenderer{vptWindow, vptDevice};
		std::vector<VPTSceneObject> vptSceneObjects;
	};

}