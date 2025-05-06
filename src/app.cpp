#include "app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>
#include <array>

namespace vi {

    struct ExamplePushConstantData {
        glm::vec2 offset;
        alignas(16) glm::vec3 color; // Data alignment is important for a lot of data in Vulkan.  See the spec.
    };

    App::App() {
        loadMeshes();

        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    App::~App() {
        vkDestroyPipelineLayout(viDevice.device(), pipelineLayout, nullptr);
    }

    void App::run() {
        while(!viWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(viDevice.device());
    }

    void App::createPipelineLayout() {        
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(ExamplePushConstantData);
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if(vkCreatePipelineLayout(viDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("[Vi] Failed to create pipeline layout");
        }
    }

    void App::createPipeline() {
        auto pipelineConfig = ViPipeline::defaultPipelineConfigInfo(viSwapChain.width(), viSwapChain.height());

        pipelineConfig.renderPass = viSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        viPipeline = std::make_unique<ViPipeline>(viDevice, "../shaders/simple.vert.spv", "../shaders/simple.frag.spv", pipelineConfig);
    }

    
    void App::createCommandBuffers() {
        commandBuffers.resize(viSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = viDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        
        if(vkAllocateCommandBuffers(viDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("[Vi] Failed to allocate command buffers");
        }

        for(int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("[Vi] Failed to start recording command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = viSwapChain.getRenderPass();
            renderPassInfo.framebuffer = viSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = viSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.3f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            viPipeline->bind(commandBuffers[i]);
            mesh->bind(commandBuffers[i]);
            
            for(int j = 0; j < 11; j++) {
                ExamplePushConstantData pushData{};
                pushData.offset = {0.1f + j * 0.05f, 0.0f - j * 0.05f};
                pushData.color = {1.0f - 0.1 * j, 1.0f - 0.1 * j, 1.0f - 0.1 * j};

                vkCmdPushConstants(commandBuffers[i], 
                                   pipelineLayout, 
                                   VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                                   0,
                                   sizeof(ExamplePushConstantData),
                                   &pushData);
                                   
                mesh->draw(commandBuffers[i]);
            }

            vkCmdEndRenderPass(commandBuffers[i]);
            
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("[Vi] Failed to record command buffer");
            }
        }
    }
    
    void App::drawFrame() {
        uint32_t imageIndex;
        auto result = viSwapChain.acquireNextImage(&imageIndex);

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("[Vi] Failed to acquire swap chain image");
        }

        result = viSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result != VK_SUCCESS) {
            throw std::runtime_error("[Vi] Failed to present swap chain image");
        }
    }

    void App::loadMeshes() {
        std::vector<ViMesh::Vertex> vertices {
            {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        mesh = std::make_unique<ViMesh>(viDevice, vertices);
    }
}