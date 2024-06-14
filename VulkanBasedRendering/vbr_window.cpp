#include "vbr_window.hpp"


namespace vbr
{
	VbrWindow::VbrWindow(const int w, const int h, const std::string name) : width(w), height(h), windowName(name)
	{
		initWindow();
	}
	VbrWindow::~VbrWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	
	void VbrWindow::initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
	}

	bool VbrWindow::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}

}


