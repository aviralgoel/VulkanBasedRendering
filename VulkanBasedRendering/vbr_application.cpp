#include "vbr_application.hpp"

vbr::VbrApplication::VbrApplication()
{
}

vbr::VbrApplication::~VbrApplication()
{
}

void vbr::VbrApplication::run()
{
	while (!vbrWindow.shouldClose())
	{
		glfwPollEvents();
	}
}
