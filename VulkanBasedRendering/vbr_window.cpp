#include "vbr_window.hpp"

// std
#include <stdexcept>

namespace vbr {

    VbrWindow::VbrWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
        initWindow();
    }

    VbrWindow::~VbrWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void VbrWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void VbrWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto vbrWindow = reinterpret_cast<VbrWindow*>(glfwGetWindowUserPointer(window));
        vbrWindow->frameBufferResized = true;
        vbrWindow->height = height;
        vbrWindow->width = width;
    }

    void VbrWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to craete window surface");
        }
    }
}  // namespace lve