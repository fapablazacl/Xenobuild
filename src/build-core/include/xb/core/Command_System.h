
#ifndef __BOK_MODEL_COMMAND_SYSTEMCOMMAND_HPP__
#define __BOK_MODEL_COMMAND_SYSTEMCOMMAND_HPP__

#include <string>
#include <vector>

#include <xb/core/Command.h>

namespace Xenobuild {
    class Command_System : public Command {
    public:
        explicit Command_System(const std::string &base);

        explicit Command_System(const std::string &base, const std::vector<std::string> &options);

        virtual void execute() override;

    private:
        const std::string _base;
        std::vector<std::string> _options;
    };
}

#endif
