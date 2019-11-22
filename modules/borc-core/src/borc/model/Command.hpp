
#ifndef __BORC_MODEL_COMMAND_COMMAND_HPP__
#define __BORC_MODEL_COMMAND_COMMAND_HPP__

#include <string>

namespace borc {
    class Command {
    public:
        virtual ~Command();

        virtual void execute() = 0;
    };
}

#endif
