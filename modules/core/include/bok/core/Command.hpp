
#ifndef __BOK_MODEL_COMMAND_COMMAND_HPP__
#define __BOK_MODEL_COMMAND_COMMAND_HPP__

#include <string>
#include <vector>

namespace bok {
    struct CommandData {
        std::string path;
        std::string name;
        std::vector<std::string> args;
    };

    class Command {
    public:
        virtual ~Command();

        [[deprecated]]
        virtual void execute() = 0;
    };
}

#endif
