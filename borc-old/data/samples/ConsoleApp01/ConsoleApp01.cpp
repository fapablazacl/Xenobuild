
#include "ConsoleApp01.hpp"

#include <iostream>

namespace ConsoleApp01 {
	int App::Run(int argc, char **argv) {
		std::cout << "Hello, world!" << std::endl;
		std::cin.get();
		return 0;
	}	
}

int main(int argc, char **argv) {
	return ConsoleApp01::App().Run(argc, argv);
}
