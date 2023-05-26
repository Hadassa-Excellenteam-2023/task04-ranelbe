#include "Controller.h"
#include <cstdlib>
#include <exception>
#include <iostream>

int main() try
{
    Controller().run();
	
	return EXIT_SUCCESS;
} 
catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return EXIT_FAILURE;
}
catch (...) {
    std::cerr << "Unknown exception\n";
    return EXIT_FAILURE;
}