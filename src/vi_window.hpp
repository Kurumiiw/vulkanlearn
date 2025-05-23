#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vi {
    class ViWindow {
        public:
            ViWindow(int w, int h, std::string title);
            ~ViWindow();

            ViWindow(const ViWindow &) = delete;
            ViWindow &operator=(const ViWindow &) = delete;

            bool shouldClose() { return glfwWindowShouldClose(window); };
            VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

        private:
            void initWindow();
            
            const int width;
            const int height;

            std::string windowTitle;

            GLFWwindow* window;
    };
}