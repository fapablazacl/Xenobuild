
#include <bok/core/package/Package.hpp>
#include <bok/core/package/Module.hpp>

namespace bok {
    Package::Package(const std::string &name) {
        this->name = name;
    }


    Package::~Package() {}


    std::vector<Module*> Package::getModules() const {
        std::vector<Module*> result;

        for (auto &component : modules) {
            result.push_back(component.get());
        }

        return result;
    }


    void Package::setVariables(const std::vector<PackageVariable> &variables) {
        this->variables = variables;
    }


    std::vector<PackageVariable> Package::getVariables() const {
        return variables;
    }


    bool Package::validateVariableMap(const std::map<std::string, std::string> &variableMap) const {
        return true;
    }
}
