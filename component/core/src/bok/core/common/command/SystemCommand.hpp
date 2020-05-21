
#ifndef __BOK_MODEL_COMMAND_SYSTEMCOMMAND_HPP__
#define __BOK_MODEL_COMMAND_SYSTEMCOMMAND_HPP__

#include <string>
#include <vector>

#include <bok/core/common/Command.hpp>

namespace bok {
    class SystemCommand : public Command {
    public:
        explicit SystemCommand(const std::string &base);

        explicit SystemCommand(const std::string &base, const std::vector<std::string> &options);

        virtual void execute() override;

    private:
        const std::string _base;
        std::vector<std::string> _options;
    };
}

#endif
