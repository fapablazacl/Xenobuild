
#include "ConfigureControllerOptions.hpp"

#include <boost/program_options.hpp>

namespace borc {
    ConfigureControllerOptions ConfigureControllerOptions::parse(int argc, char **argv) {
        boost::program_options::options_description desc("Allowed options for Configure subcommand");

        desc.add_options()
            ("help", "produce this message")
            ("build-type", boost::program_options::value<std::string>(), "set build type (debug, release, all)")
            ("toolchain", boost::program_options::value<std::string>(), "set toolchain (gcc, vc, clang)")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        ConfigureControllerOptions options;

        if (vm.count("help")) {
            std::stringstream ss;

            ss << desc << "\n";

            options.showHelp = true;
            options.helpMessage = ss.str();
        }
        
        if (vm.count("build-type")) {
            options.buildType = vm["build-type"].as<std::string>();
        }

        if (vm.count("toolchain")) {
            options.toolchain = vm["toolchain"].as<std::string>();
        }

        return options;
    }
}
