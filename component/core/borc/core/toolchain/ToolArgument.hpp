
#pragma once 

#include <string>

namespace borc {
    struct ToolArgument {
        ~ToolArgument();
            
        std::string evaluate(const std::string &value);

        std::string name;
        std::string template_;
    };
}
