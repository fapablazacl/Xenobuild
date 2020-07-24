
#ifndef __BOK_MODEL_COMMAND_COMMAND_HPP__
#define __BOK_MODEL_COMMAND_COMMAND_HPP__

#include <string>

namespace bok {
    class Command {
    public:
        virtual ~Command();

        virtual void execute() = 0;
    };
}

#endif
