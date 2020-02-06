
#pragma once 

#include <memory>
#include <vector>
#include <string>

namespace borc {
    struct Tool;

    class Toolchain {
    public:
        ~Toolchain();

        std::string name;
        std::vector<std::unique_ptr<Tool>> tools;
    };
}
