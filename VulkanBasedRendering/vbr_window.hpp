#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace vbr {

	class VbrWindow {
	public:
		VbrWindow(int w, int h, std::string name);
		~VbrWindow();

		VbrWindow(const VbrWindow&) = delete;
		VbrWindow& operator=(const VbrWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
}  // namespace lve