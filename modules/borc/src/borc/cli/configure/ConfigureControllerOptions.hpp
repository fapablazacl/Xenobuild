
#ifndef __BORC_CLI_CONFIGURECONTROLLEROPTIONS_HPP__
#define __BORC_CLI_CONFIGURECONTROLLEROPTIONS_HPP__

#include <string>
#include <boost/optional.hpp>

namespace borc {
    struct ConfigureControllerOptions {
        bool showHelp = false;
        std::string helpMessage;

        boost::optional<std::string> buildType;
        boost::optional<std::string> toolchain;

        static ConfigureControllerOptions parse(int argc, char **argv);
    };
}

#endif
