#include <app/app.h>
#include <graphic/vulkan.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

const uint32_t width = 1920;
const uint32_t height = 1080;

int main()
{
	auswahlen::app application(width, height);
	try
	{
		application.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}