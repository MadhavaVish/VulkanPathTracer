#include "VPTWindow.hpp"

namespace VPT {
	VPTWindow::VPTWindow(int w, int h, std::string name) : width(w), height(h), windowName(name) {
		initWindow();
	}

	VPTWindow::~VPTWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void VPTWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}