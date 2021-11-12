
#include <bok/core/CommandFactory.hpp>

#include <iostream>
#include <boost/filesystem.hpp>
#include <bok/core/Command_System.hpp>
#include <bok/core/Command_Folder.hpp>

namespace bok {
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
