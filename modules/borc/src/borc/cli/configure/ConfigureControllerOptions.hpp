
#ifndef __BORC_CLI_CONFIGURECONTROLLEROPTIONS_HPP__
#define __BORC_CLI_CONFIGURECONTROLLEROPTIONS_HPP__

#include <map>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

namespace borc {
    struct ConfigureControllerOptions {
        bool showHelp = false;
        std::string helpMessage;

        std::string buildType;
        boost::optional<std::string> toolchain;

        //! additional search path for package dependencies        
        std::vector<std::string> searchPaths;

        boost::optional<boost::filesystem::path> sourcePath;
        boost::optional<boost::filesystem::path> outputPath;

        std::map<std::string, std::string> variables;

        static ConfigureControllerOptions parse(int argc, char **argv);
    };
}

#endif
