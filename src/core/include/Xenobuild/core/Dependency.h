#pragma once 

#include <string>
#include <map>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

#include "Util.h"

namespace Xenobuild {
    struct CommandX;
    struct CommandResult;
    
    class CommandExecutor;
    
    
    struct GitRepository {
        std::string url;
        std::string tag;

        GitRepository(const std::string& url, const std::string& tag);

        CommandResult clone(CommandExecutor &execute, const boost::filesystem::path &sourcePath) const;
    };

    struct CMakeDefinition {
        std::string name;
        std::string value;
    };

    struct CMakeConfig {
        std::string sourcePath;
        std::string buildPath;
        std::string generator;
        std::map<std::string, std::string> definitions;
    };

    struct CMakeBuild {
        std::string buildPath;
        boost::optional<unsigned> jobCount;
    };

    struct CMakeInstall {
        std::string buildPath;
    };

    enum class CMakeBuildType {
        Default,
        Debug,
        Release
    };

    struct Dependency {
        //! the Git URL repository
        std::string url;

        //! the tag (or branch), of the Git repository.
        std::string tag;

        //! version string. used to assemble the installation path suffix.
        std::string version;

        //! cmake definitions, in the dependency build/install stage.
        std::map<std::string, std::string> definitions;

        Dependency() {}

        Dependency(const std::string& url)
            : url(url) {}

        Dependency(const std::string& url, const std::string &tag, const std::string &version)
            : url(url), tag(tag), version(version) {}

        Dependency(const std::string& url, const std::string &tag, const std::string &version, const std::map<std::string, std::string> &definitions)
            : url(url), tag(tag), version(version), definitions(definitions) {}
        
        Dependency(const std::string& url, const std::map<std::string, std::string> &definitions)
            : url(url), definitions(definitions) {}
    };
}
