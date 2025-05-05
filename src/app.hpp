#pragma once

#include "vi_window.hpp"
#include "vi_pipeline.hpp"

namespace vi {
    class App {
        public:
            static constexpr int WIDTH = 1920;
            static constexpr int HEIGHT = 1080;

            void run();
        private:
            ViWindow viWindow{WIDTH, HEIGHT, "Vi"};
            ViPipeline viPipeline{"../shaders/simple.vert.spv", "../shaders/simple.frag.spv"};
    };
}