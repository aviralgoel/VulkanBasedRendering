#include "vbr_application.hpp"
#include <stdexcept>
#include <iostream>
#include <cstdlib>

int main()
{	
	vbr::VbrApplication MyApplication{};
	
	try {
		MyApplication.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}