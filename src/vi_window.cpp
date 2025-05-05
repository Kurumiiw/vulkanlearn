#include "vi_window.hpp"

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
}