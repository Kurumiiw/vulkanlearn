#pragma once

#include "vi_device.hpp"
#include "vi_pipeline.hpp"
#include "vi_swap_chain.hpp"
#include "vi_window.hpp"

//std
#include <memory>
#include <vector>

namespace vi {
    class App {
        public:
            static constexpr int WIDTH = 1920;
            static constexpr int HEIGHT = 1080;

            App();
            ~App();

            App(const App&) = delete;
            App &operator=(const App&) = delete;

            void run();
        private:
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void drawFrame();

            ViWindow viWindow{WIDTH, HEIGHT, "Vi"};
            ViDevice viDevice{viWindow};
            ViSwapChain viSwapChain{viDevice, viWindow.getExtent()};
            std::unique_ptr<ViPipeline> viPipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
    };
}