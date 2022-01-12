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
	}

	std::unique_ptr<VPTModel> createCubeModel(VPTDevice &device, glm::vec3 offset)
	{
		VPTModel::Builder modelBuilder{};

		modelBuilder.vertices = {

			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto &v : modelBuilder.vertices)
		{
			v.position += offset;
		}
		modelBuilder.indices = {0, 1, 2, 0, 3, 1, 4, 5, 6, 4, 7, 5, 8, 9, 10, 8, 11, 9,
								12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
		return std::make_unique<VPTModel>(device, modelBuilder);
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