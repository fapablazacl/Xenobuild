
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "borc/cli/Controller.hpp"
#include "borc/cli/ControllerFactory.hpp"


int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            throw std::runtime_error("No subcommand supplied.");
        }

        std::vector<char*> args = {argv + 1, argv + argc};

        borc::ControllerFactory controllerFactory;
        auto controller = controllerFactory.create(args[0]);

        controller->perform(args.size(), args.data());
    
        return 0;
    } catch (const std::exception &exp) {
        std::cout << "Exception caught:\n" << exp.what() << std::endl;

        return 1;
    }

    return 0;
}

// configure --toolchain=vc --build-type=all --var=BoostPath:C:\Boost --var=BoostVersion:1.7.1
