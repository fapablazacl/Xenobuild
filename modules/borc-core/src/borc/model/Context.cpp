
#include <borc/model/Context.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Package.hpp>

namespace borc {
    Context::Context() {}

    Context::~Context() {}

    void Context::registerPackage(const Package *package) {
        packages.push_back(package);

        for (const Module *module : package->getModules()) {
            const std::string key = this->getModuleIdentifier(module);
            moduleMap.insert({key, module});
        }
    }

    void Context::unregisterPackage(const Package *package) {
        // TODO: Add implementation
        throw std::runtime_error("Context::unregisterPackage: Not implemented");
    }

    std::string Context::getModuleIdentifier(const Module *module) const {
        return module->getPackage()->getName() + "/" + module->getName();
    }

    const Module* Context::findModule(const std::string &identifier) const {
        auto it = moduleMap.find(identifier);

        if (it == moduleMap.end()) {
            return nullptr;
        }

        return it->second;
    }
}
