
#ifndef __BORC_CLI_BUILDCONTROLLEROPTIONS_HPP__
#define __BORC_CLI_BUILDCONTROLLEROPTIONS_HPP__

#include <string>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

namespace bok {
    struct BuildControllerOptions {
        bool force = false;
        bool showHelp = false;
        std::string helpMessage;

        boost::optional<boost::filesystem::path> sourcePath;
        boost::optional<boost::filesystem::path> outputPath;

        static BuildControllerOptions parse(int argc, char **argv);
    };
}

#endif
