
#include "BuildControllerOptions.hpp"

#include <boost/program_options.hpp>

namespace borc {
    BuildControllerOptions BuildControllerOptions::parse(int argc, char **argv) {
        boost::program_options::options_description desc("Allowed options for Configure subcommand");

        desc.add_options()
            ("help", "produce help message")
            ("force,f", "Force a rebuild")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        BuildControllerOptions options;

        if (vm.count("help")) {
            std::stringstream ss;

            ss << desc << "\n";

            options.showHelp = true;
            options.helpMessage = ss.str();
        }
        
        if (vm.count("force")) {
            options.force = true;
        }

        return options;
    }
}
