
#ifndef __BOK_MODEL_COMMAND_FOLDERCOMMAND_HPP__
#define __BOK_MODEL_COMMAND_FOLDERCOMMAND_HPP__

#include <bok/model/Command.hpp>
#include <bok/model/CommandFactory.hpp>
#include <boost/filesystem/path.hpp>

namespace bok {
    class FolderCommand : public Command {
    public:
        explicit FolderCommand(const boost::filesystem::path &path, const PathCommand pathCommand);

        virtual void execute() override;

    private:
        boost::filesystem::path path;
        PathCommand pathCommand;
    };
}

#endif
