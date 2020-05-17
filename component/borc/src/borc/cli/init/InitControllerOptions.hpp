
#ifndef __BORC_CLI_INITCONTROLLEROPTIONS_HPP__
#define __BORC_CLI_INITCONTROLLEROPTIONS_HPP__

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

namespace bok {
    struct InitControllerOptions {
        bool showHelp = false;
        std::string helpMessage;

        boost::filesystem::path path;
        std::string packageName;
        std::string moduleType;

        static InitControllerOptions parse(int argc, char **argv);
    };
}

#endif
