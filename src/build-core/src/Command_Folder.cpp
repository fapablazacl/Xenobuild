
#include <bok/core/Command_Folder.hpp>

#include <boost/filesystem.hpp>

namespace bok {
    Command_Folder::Command_Folder(const boost::filesystem::path &path, const PathCommand pathCommand) {
        this->path = path;
        this->pathCommand = pathCommand;
    }


    void Command_Folder::execute() {
        switch (pathCommand) {
            case PathCommand::Create: 
                boost::filesystem::create_directories(path); 
                break;
        }
    }
}
