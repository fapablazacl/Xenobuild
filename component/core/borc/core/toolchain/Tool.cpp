
#include "Tool.hpp"

namespace borc {
    Tool::Tool(const Toolchain *toolchain_, const std::string &name_) 
        : toolchain(toolchain_), name(name_) {}


    Tool::~Tool() {}


    Tool* Tool::setCommandName(const std::string &value) {
        commandName = value;

        return this;
    }


    Tool* Tool::setCommandPath(const std::string &value) {
        commandPath =  value;

        return this;
    }


    Tool* Tool::setArguments(const std::vector<ToolArgument> &values) {
        arguments = values;

        return this;
    }
}
