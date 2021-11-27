
#ifndef __BOK_MODEL_COMMAND_FOLDERCOMMAND_HPP__
#define __BOK_MODEL_COMMAND_FOLDERCOMMAND_HPP__

#include <xb/core/Command.h>
#include <xb/core/CommandFactory.h>
#include <boost/filesystem/path.hpp>

namespace Xenobuild {
    class Command_Folder : public Command {
    public:
        explicit Command_Folder(const boost::filesystem::path &path, const PathCommand pathCommand);

        virtual void execute() override;

    private:
        boost::filesystem::path path;
        PathCommand pathCommand;
    };
}

#endif
