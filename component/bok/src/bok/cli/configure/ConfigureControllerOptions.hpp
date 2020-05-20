
#ifndef __BOK_CLI_CONFIGURECONTROLLEROPTIONS_HPP__
#define __BOK_CLI_CONFIGURECONTROLLEROPTIONS_HPP__

#include <map>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

namespace bok {
    struct ConfigureControllerOptions {
        bool showHelp = false;
        std::string helpMessage;

        std::string buildType;
        boost::optional<std::string> toolchain;
        boost::optional<std::string> toolchainPath;

        //! additional search path for package dependencies
        std::vector<std::string> searchPaths;

        boost::optional<boost::filesystem::path> sourcePath;
        boost::optional<boost::filesystem::path> outputPath;

        std::map<std::string, std::string> variables;

        static ConfigureControllerOptions parse(int argc, char **argv);
    };
}

#endif
