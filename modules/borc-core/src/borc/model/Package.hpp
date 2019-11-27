
#ifndef __BORC_MODEL_PACKAGE_HPP__
#define __BORC_MODEL_PACKAGE_HPP__

#include <string>
#include <vector>
#include <memory>

#include "Language.hpp"

namespace borc {
    class Module;
    class Package {
    public:
        Package(const std::string &name);

        ~Package();

        std::string getName() const {
            return name;
        }

        std::vector<Module*> getModules() const;

        template<typename ModuleImpl>
        ModuleImpl* createModule() {
            auto module = new ModuleImpl(this);

            modules.emplace_back(module);

            return module;
        }

    private:
        std::string name;
        std::vector<std::unique_ptr<Module>> modules;
    };
}

#endif
