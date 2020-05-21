
#include "InitControllerOptions.hpp"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace bok {
    InitControllerOptions InitControllerOptions::parse(int argc, char **argv) {
        boost::program_options::options_description desc("Allowed options for Configure subcommand");

        desc.add_options()
            ("help", "produce this message")
            ("path", boost::program_options::value<std::string>(), "Configuration path. Defaults to the current folder")
            ("name", boost::program_options::value<std::string>(), "The package name to be created. By default")
            ("type", boost::program_options::value<std::string>(), "The default component type to be created. Can be application/cli, application/gui, library/static or library/dynamic")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        InitControllerOptions options;

        if (vm.count("help")) {
            std::stringstream ss;

            ss << desc << "\n";

            options.showHelp = true;
            options.helpMessage = ss.str();
        }
        
        if (vm.count("path")) {
            options.path = vm["path"].as<boost::filesystem::path>();
        } else {
            options.path = boost::filesystem::current_path();
        }

        if (vm.count("name")) {
            options.packageName = vm["name"].as<std::string>();
        }

        if (vm.count("type")) {
            options.moduleType = vm["type"].as<std::string>();
        }

        return options;
    }
}
