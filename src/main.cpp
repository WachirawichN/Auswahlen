#include <graphic/window.h>
#include <simulation/simulation.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
	auswahlen::simulation simulation;
	try
	{
		simulation.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}