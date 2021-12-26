#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

namespace VPT
{
	class VPTWindow
	{
	public:
		VPTWindow(int w, int h, std::string name);
		~VPTWindow();
		
		VPTWindow(const VPTWindow &) = delete;
		VPTWindow &operator=(const VPTWindow &) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); };
		VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; };
		bool wasWindowResized(){return frameBufferResized;}
		void resetWindowResizedFlag(){frameBufferResized = false;}
		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
		GLFWwindow *getGLFWWindow() const { return window; }
	private:
		static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);
		void initWindow();

		int height;
		int width;
		bool frameBufferResized;

		std::string windowName;
		GLFWwindow *window;
	};
}