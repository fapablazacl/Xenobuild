
#pragma once 

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace borc {
    class Tool;

    class Toolchain {
    public:
        explicit Toolchain(const std::string &name);

        ~Toolchain();

        std::string getName() const {
            return name;
        }

        std::string getDescription() const {
            return description;
        }

        Tool* createTool(const std::string &name);

        Tool* getTool(const std::string &name);

        int getToolCount() const;

    private:
        std::string name;
        std::string description;

        std::map<std::string, std::unique_ptr<Tool>> tools;
    };
}
