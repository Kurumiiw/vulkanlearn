#include "vi_mesh.hpp"

#include <cassert>
#include <cstring>

namespace vi {
    ViMesh::ViMesh(ViDevice& device, const std::vector<Vertex>& vertices) : device{device} {
        createVertexBuffers(vertices);
    }

    ViMesh::~ViMesh() {
        vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
        vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
    }

    void ViMesh::createVertexBuffers(const std::vector<Vertex> &vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        device.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer,
            vertexBufferMemory
        );

        void* data;
        
        vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device.device(), vertexBufferMemory);
    }

    void ViMesh::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer, vertexBuffer};
        VkDeviceSize offsets[] = {0, 0};

        vkCmdBindVertexBuffers(commandBuffer, 0, 2, buffers, offsets);
    }
    
    void ViMesh::draw(VkCommandBuffer commandBuffer) {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    
    std::vector<VkVertexInputBindingDescription> ViMesh::Vertex::getBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(2);

        bindingDescriptions[0].binding   = 0;
        bindingDescriptions[0].stride    = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        bindingDescriptions[1].binding   = 1;
        bindingDescriptions[1].stride    = sizeof(Vertex);
        bindingDescriptions[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> ViMesh::Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

        attributeDescriptions[0].binding   = 0;
        attributeDescriptions[0].location  = 0;
        attributeDescriptions[0].format    = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset    = 0;
        
        attributeDescriptions[1].binding   = 1;
        attributeDescriptions[1].location  = 1;
        attributeDescriptions[1].format    = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset    = offsetof(ViMesh::Vertex, color);
        return attributeDescriptions;
    }
}