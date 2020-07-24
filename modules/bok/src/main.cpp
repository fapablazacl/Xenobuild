
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include <bok/cli/Controller.hpp>
#include <bok/cli/ControllerFactory.hpp>


int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            throw std::runtime_error("No subcommand supplied.");
        }

        std::vector<char*> args = {argv + 1, argv + argc};

        bok::ControllerFactory controllerFactory;
        auto controller = controllerFactory.create(args[0]);

        controller->perform(args.size(), args.data());
    
        return EXIT_SUCCESS;
    }
    catch (std::runtime_error exp) {
        std::cout << exp.what()  << std::endl;

        return EXIT_FAILURE;
    }
    catch (std::exception exp) {
        std::cout << "Exception caught:\n" << exp.what() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// configure --toolchain=vc --build-type=all --var=BoostPath:C:\Boost --var=BoostVersion:1.7.1
