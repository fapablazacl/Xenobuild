
#include <bok/core/CommandFactory.hpp>

#include <iostream>
#include <boost/filesystem.hpp>

#include "SystemCommand.hpp"
#include "FolderCommand.hpp"

namespace bok {
    CommandFactory::~CommandFactory() {}


    Command* CommandFactory::createCommand(const std::string &base, const std::vector<std::string> &options) {
        auto command = new SystemCommand(base, options);

        _cachedCommands.emplace_back(command);

        return command;
    }


    Command* CommandFactory::createPathCommand(const boost::filesystem::path &path, const PathCommand pathCommand) {
        auto command = new FolderCommand(path, pathCommand);

        _cachedCommands.emplace_back(command);

        return command;
    }
}
