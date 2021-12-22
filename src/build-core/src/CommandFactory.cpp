
#include <Xenobuild/core/CommandFactory.h>

#include <iostream>
#include <boost/filesystem.hpp>
#include <Xenobuild/core/Command_System.h>
#include <Xenobuild/core/Command_Folder.h>

namespace Xenobuild {
    CommandFactory::~CommandFactory() {}


    Command* CommandFactory::createCommand(const std::string &base, const std::vector<std::string> &options) {
        auto command = new Command_System(base, options);

        _cachedCommands.emplace_back(command);

        return command;
    }


    Command* CommandFactory::createPathCommand(const boost::filesystem::path &path, const PathCommand pathCommand) {
        auto command = new Command_Folder(path, pathCommand);

        _cachedCommands.emplace_back(command);

        return command;
    }
}
