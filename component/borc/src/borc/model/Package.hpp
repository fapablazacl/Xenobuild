
#ifndef __BORC_MODEL_PACKAGE_HPP__
#define __BORC_MODEL_PACKAGE_HPP__

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "Language.hpp"

namespace bok {
    struct PackageVariable {
        std::string name;

        bool operator== (const PackageVariable &other) const {
            return name == other.name;
        }

        bool operator!= (const PackageVariable &other) const {
            return name != other.name;
        }
    };

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

        void setVariables(const std::vector<PackageVariable> &variables);

        std::vector<PackageVariable> getVariables() const;

        bool hasVariables() const {
            return variables.size() > 0;
        }

        bool validateVariableMap(const std::map<std::string, std::string> &variableMap) const;

    private:
        std::string name;
        std::vector<std::unique_ptr<Module>> modules;
        std::vector<PackageVariable> variables;
    };
}

#endif
