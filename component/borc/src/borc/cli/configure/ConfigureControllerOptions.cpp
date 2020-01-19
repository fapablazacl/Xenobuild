
#include "ConfigureControllerOptions.hpp"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

namespace borc {
    ConfigureControllerOptions ConfigureControllerOptions::parse(int argc, char **argv) {
        namespace po = boost::program_options;

        po::options_description desc("Allowed options for Configure subcommand");

        desc.add_options()
            ("help", "produce this message")
            ("build-type", po::value<std::string>(), "set build type (debug, release, all)")
            ("toolchain", po::value<std::string>(), "set toolchain (gcc, vc)")
            ("toolchain-path", po::value<std::string>(), "set toolchain installation path")
            ("search-path", po::value<std::string>(), "set current search path directory for packages(?)")
            ("var", po::value<std::vector<std::string>>(), "define required variable used by some packages")
        ;

        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        ConfigureControllerOptions options;

        if (vm.count("help")) {
            std::stringstream ss;

            ss << desc << "\n";

            options.showHelp = true;
            options.helpMessage = ss.str();
        }
        
        if (vm.count("build-type")) {
            options.buildType = vm["build-type"].as<std::string>();
        } else {
            options.buildType = "all";
        }

        if (vm.count("toolchain")) {
            options.toolchain = vm["toolchain"].as<std::string>();
        }

        if (vm.count("toolchain-path")) {
            options.toolchainPath = vm["toolchain-path"].as<std::string>();
        }

        if (vm.count("search-path")) {
            options.searchPaths.push_back(vm["search-path"].as<std::string>());
        }

        if (vm.count("var")) {
            auto vars = vm["var"].as<std::vector<std::string>>();

            for (const auto &var : vars) {
                std::vector<std::string> keyValue;
                boost::split(keyValue, var, boost::is_any_of(":"));

                const size_t size = keyValue.size();

                switch (size) {

                case 2:
                    options.variables.insert({keyValue[0], keyValue[1]});
                    break;

                case 3:
                    options.variables.insert({keyValue[0], keyValue[1] + ":" + keyValue[2]});
                    break;
                }
            }
        }

        return options;
    }
}
