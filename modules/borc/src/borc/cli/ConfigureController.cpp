
#include "ConfigureController.hpp"

#include <boost/program_options.hpp>
#include <iostream>

namespace borc {
    ConfigureController::~ConfigureController() {}

    void ConfigureController::perform(int argc, char **argv) {
        boost::program_options::options_description desc("Allowed options for Configure subcommand");
        desc.add_options()
            ("help", "produce help message")
            ("build-type", boost::program_options::value<std::string>(), "set build type (Debug, Release, All)")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);    

        if (vm.count("help")) {
            std::cout << desc << "\n";
        }

        if (vm.count("build-type")) {
            std::cout << "Build type was set to " << vm["build-type"].as<std::string>() << ".\n";
        } else {
            std::cout << "Build type defaulted to All.\n";
        }
    }
}
