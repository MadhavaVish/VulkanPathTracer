#pragma once

#define GLFW_INCLUDE_VULKAN


#include <GLFW/glfw3.h>
#include <string>


namespace VPT {
	class VPTWindow {
	public:
		VPTWindow(int w, int h, std::string name);
		~VPTWindow();

		VPTWindow(const VPTWindow&) = delete;
		VPTWindow& operator=(const VPTWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); };
	private:
		void initWindow();

		const int height;
		const int width;

		std::string windowName;
		GLFWwindow* window;
	};
}