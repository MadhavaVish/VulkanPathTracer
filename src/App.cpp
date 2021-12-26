#include "App.hpp"

#include "VPTCamera.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"

#include <stdexcept>
#include <array>
#include <cassert>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace VPT
{
	App::App()
	{
		loadSceneObjects();
	}
	App::~App()
	{
	}
	void App::run()
	{
		SimpleRenderSystem simpleRenderSystem{vptDevice, vptRenderer.getSwapChainRenderPass()};
		VPTCamera camera{};
		auto viewerObject = VPTSceneObject::createSceneObject();

		KeyboardMovementController cameraController{};
		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!vptWindow.shouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			cameraController.moveInPlaneXZ(vptWindow.getGLFWWindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = vptRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1, 10);
			if (auto commandBuffer = vptRenderer.beginFrame())
			{
				vptRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderSceneObjects(commandBuffer, vptSceneObjects, camera);
				vptRenderer.endSwapChainRenderPass(commandBuffer);
				vptRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(vptDevice.device());
		// VPTImgui vptImgui{
		// 	vptWindow,
		// 	vptDevice,
		// 	vptRenderer.getSwapChainRenderPass(),
		// 	vptRenderer.getImageCount()};

		// SimpleRenderSystem simpleRenderSystem{vptDevice, vptRenderer.getSwapChainRenderPass()};
		// while (!vptWindow.shouldClose()) {
		// 	glfwPollEvents();

		// 	if (auto commandBuffer = vptRenderer.beginFrame()) {
		// 		// tell imgui that we're starting a new frame
		// 		vptImgui.newFrame();

		// 		vptRenderer.beginSwapChainRenderPass(commandBuffer);

		// 		// render game objects first, so they will be rendered in the background. This
		// 		// is the best we can do for now.
		// 		// Once we cover offscreen rendering, we can render the scene to a image/texture rather than
		// 		// directly to the swap chain. This texture of the scene can then be rendered to an imgui
		// 		// subwindow
		// 		simpleRenderSystem.renderSceneObjects(commandBuffer, vptSceneObjects);

		// 		// example code telling imgui what windows to render, and their contents
		// 		// this can be replaced with whatever code/classes you set up configuring your
		// 		// desired engine UI
		// 		vptImgui.runExample();

		// 		// as last step in render pass, record the imgui draw commands
		// 		vptImgui.render(commandBuffer);

		// 		vptRenderer.endSwapChainRenderPass(commandBuffer);
		// 		vptRenderer.endFrame();
		// 	}
		// }
	}

	std::unique_ptr<VPTModel> createCubeModel(VPTDevice &device, glm::vec3 offset)
	{
		std::vector<VPTModel::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto &v : vertices)
		{
			v.position += offset;
		}
		return std::make_unique<VPTModel>(device, vertices);
	}

	void App::loadSceneObjects()
	{
		std::shared_ptr<VPTModel> vptModel = createCubeModel(vptDevice, {.0f, .0f, .0f});
		auto cube = VPTSceneObject::createSceneObject();
		cube.model = vptModel;
		cube.transform.translation = {0.f, 0.f, 2.5f};
		cube.transform.scale = {.5f, .5f, .5f};
		vptSceneObjects.push_back(std::move(cube));
	}
}