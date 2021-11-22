#pragma once

#include "VPTDevice.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace VPT
{

    class VPTModel
    {
    public:
        struct Vertex{
            glm::vec2 position;
            glm::vec3 color;
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        };
        VPTModel(VPTDevice& device, const std::vector<Vertex> &vertices);
        ~VPTModel();
        
        VPTModel(const VPTModel &) = delete;
        VPTModel &operator=(const VPTModel &) = delete;

        void bind(VkCommandBuffer VkCommandBuffer);
        void draw(VkCommandBuffer VkCommandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        VPTDevice &vptDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}