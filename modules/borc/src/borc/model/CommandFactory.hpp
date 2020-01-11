
#ifndef __BORC_MODEL_COMMAND_COMMANDFACTORY_HPP__
#define __BORC_MODEL_COMMAND_COMMANDFACTORY_HPP__

#include <string>
#include <vector>
#include <memory>
#include <boost/filesystem/path.hpp>

namespace borc {
    enum class PathCommand {
        Create,
        Delete
    };

    class Command;
    class CommandFactory {
    public:
        ~CommandFactory();

        Command* createCommand(const std::string &base, const std::vector<std::string> &options = {});

        Command* createPathCommand(const boost::filesystem::path &path, const PathCommand pathCommand);

    private:
        std::vector<std::unique_ptr<Command>> _cachedCommands;
    };
}

#endif
