
#pragma once 

#include <string>
#include <optional>
#include <vector>
#include <map>

#include "ToolArgument.hpp"

namespace borc {
    class Toolchain;
    class Tool {
    public:
        explicit Tool(const Toolchain *toolchain, const std::string &name);

        ~Tool();

        std::string getName() const {
            return name;
        }

        std::string getCommandName() const {
            return commandName;
        }

        std::optional<std::string> getCommandPath() const {
            return commandPath;
        }

        std::vector<ToolArgument> getArguments() const {
            return arguments;
        }

        const Toolchain *getToolchain() const {
            return toolchain;
        }

        Tool* setCommandName(const std::string &value);

        Tool* setCommandPath(const std::string &value);

        Tool* setArguments(const std::vector<ToolArgument> &values);

    private:
        std::string name;
        std::string commandName;
        const Toolchain *toolchain = nullptr;

        /**
         * @brief The full path where this command-line tool is located
         * @todo Rethink this attribute. We can manipulathe the PATH environment variable in order to select at runtime the path of the different commands.
         */
        std::optional<std::string> commandPath;
        std::vector<ToolArgument> arguments;
    };
}
