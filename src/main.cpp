#include <app/app.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

const uint32_t width = 800;
const uint32_t height = 600;

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