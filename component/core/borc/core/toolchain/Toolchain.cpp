
#include "Toolchain.hpp"

#include <stdexcept>
#include "Tool.hpp"

namespace borc {
    Toolchain::Toolchain(const std::string &name_) : name(name_) {}


    Toolchain::~Toolchain() {}


    Tool* Toolchain::createTool(const std::string &name) {
        if (auto it = tools.find(name); it != tools.end()) {
            throw std::runtime_error("Can't create tool with name '" + name + "', because already exist another one in the Toolchain '" + this->name + "'");
        }

        auto tool = new Tool(this, name);

        tools[name] = std::unique_ptr<Tool>(tool);

        return tool;
    }


    Tool* Toolchain::getTool(const std::string &name) {
        if (auto it = tools.find(name); it != tools.end()) {
            return it->second.get();
        }

        return nullptr;
    }


    int Toolchain::getToolCount() const {
        return static_cast<int>(tools.size());
    }
}
