
#include <borc/model/Package.hpp>
#include <borc/model/Module.hpp>

namespace borc {
    Package::Package(const std::string &name) {
        this->name = name;
    }


    Package::~Package() {}


    std::vector<Module*> Package::getModules() const {
        std::vector<Module*> result;

        for (auto &module : modules) {
            result.push_back(module.get());
        }

        return result;
    }


    Module* Package::createModule() {
        auto module = new Module(this);

        modules.emplace_back(module);

        return module;
    }
}
