#pragma once

#include <string>
#pragma once

#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#include <string>

namespace vbr {
	class VbrWindow {

	public:
		VbrWindow(const int w, const int h, const std::string name);
		~VbrWindow();
		void initWindow();
		bool shouldClose();

		VbrWindow(const VbrWindow&) = delete;
		VbrWindow& operator= (const VbrWindow&) = delete;

	private:
		const uint32_t width;
		const uint32_t height;
		const std::string windowName;
		GLFWwindow* window;

	};
}

