#include "App.hpp"

namespace VPT {
	void App::run() {
		while (!vptWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}