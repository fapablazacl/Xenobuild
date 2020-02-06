
#include "ToolArgument.hpp"

namespace borc {
    ToolArgument::~ToolArgument() {}
    

    std::string ToolArgument::evaluate(const std::string &value) {
        return template_;
    }
}
