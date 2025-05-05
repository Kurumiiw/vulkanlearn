#include "vi_window.hpp"

#include <stdexcept>

namespace vi {
    ViWindow::ViWindow(int w, int h, std::string title) : width{w}, height{h}, windowTitle{title} {
        initWindow();
    }

    ViWindow::~ViWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void ViWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    }

    void ViWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("[Vi] Failed to create window surface");
        }
    }
}