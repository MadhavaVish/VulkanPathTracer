#include "VPTWindow.hpp"

#include <stdexcept>

namespace VPT
{
	VPTWindow::VPTWindow(int w, int h, std::string name) : width(w), height(h), windowName(name)
	{
		initWindow();
	}

	VPTWindow::~VPTWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void VPTWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
	}

	void VPTWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface");
		}
	}

	void VPTWindow::frameBufferResizedCallback(GLFWwindow *window, int width, int height)
	{
		auto vptWindow = reinterpret_cast<VPTWindow*>(glfwGetWindowUserPointer(window));
		vptWindow->frameBufferResized = true;
		vptWindow->width = width;
		vptWindow->height = height;
	}
}