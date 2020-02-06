
#pragma once 

#include <string>
#include <optional>
#include <vector>

#include "ToolArgument.hpp"

namespace borc {
    class Tool {
    public:
        ~Tool();

        std::string name;
        std::string commandName;
        std::optional<std::string> commandPath;

        std::vector<ToolArgument> arguments;
    };
}
