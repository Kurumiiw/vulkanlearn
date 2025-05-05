#include "app.hpp"

namespace vi {
    void App::run() {
        while(!viWindow.shouldClose()) {
            glfwPollEvents();
        }
    }
}