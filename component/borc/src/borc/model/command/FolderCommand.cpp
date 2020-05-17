
#include "FolderCommand.hpp"

#include <boost/filesystem.hpp>

namespace bok {
    FolderCommand::FolderCommand(const boost::filesystem::path &path, const PathCommand pathCommand) {
        this->path = path;
        this->pathCommand = pathCommand;
    }


    void FolderCommand::execute() {
        switch (pathCommand) {
            case PathCommand::Create: 
                boost::filesystem::create_directories(path); 
                break;
        }
    }
}
