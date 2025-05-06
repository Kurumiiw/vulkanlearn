#pragma once

#include "vi_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace vi {
    class ViMesh {
        public:

        struct Vertex {
            glm::vec2 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        ViMesh(ViDevice& device, const std::vector<Vertex>& vertices);
        ~ViMesh();

        ViMesh(const ViMesh&) = delete;
        ViMesh &operator=(const ViMesh&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        ViDevice& device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}