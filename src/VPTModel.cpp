#include "VPTModel.hpp"

#include <cassert>

namespace VPT
{
    VPTModel::VPTModel(VPTDevice &device, const std::vector<Vertex> &vertices)
        : vptDevice(device)
    {
        createVertexBuffers(vertices);
    }

    VPTModel::~VPTModel()
    {
        vkDestroyBuffer(vptDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(vptDevice.device(), vertexBufferMemory, nullptr);
    }
    void VPTModel::createVertexBuffers(const std::vector<Vertex> &vertices)
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0])*vertexCount;

        vptDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer, 
            vertexBufferMemory
        );
        void* data;
        vkMapMemory(vptDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(vptDevice.device(), vertexBufferMemory);
    }
    void VPTModel::bind(VkCommandBuffer VkCommandBuffer)
    {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(VkCommandBuffer, 0, 1, buffers, offsets);
    }

    void VPTModel::draw(VkCommandBuffer VkCommandBuffer)
    {
        vkCmdDraw(VkCommandBuffer, vertexCount, 1, 0, 0);
    }

    std::vector<VkVertexInputBindingDescription> VPTModel::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> VPTModel::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = 0;
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;        
    }

}
