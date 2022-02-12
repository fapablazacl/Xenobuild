
#pragma once 

#include <string>
#include <boost/filesystem/path.hpp>

namespace Xenobuild {
    class CommandExecutor;
    struct CommandResult;

    struct GitRepository {
        std::string url;
        std::string tag;

        GitRepository(const std::string& url, const std::string& tag);

        CommandResult clone(CommandExecutor &execute, const boost::filesystem::path &sourcePath) const;
    };
}
