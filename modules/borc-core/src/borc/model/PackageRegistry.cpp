
#include <borc/model/PackageRegistry.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Package.hpp>

namespace borc {
    PackageRegistry::PackageRegistry() {}

    PackageRegistry::~PackageRegistry() {}

    void PackageRegistry::registerPackage(std::unique_ptr<Package> package) {
        for (const Module *module : package->getModules()) {
            const std::string key = this->getModuleIdentifier(module);
            moduleMap.insert({key, module});
        }

        packages.push_back(std::move(package));
    }


    std::string PackageRegistry::getModuleIdentifier(const Module *module) const {
        return module->getPackage()->getName() + "/" + module->getName();
    }


    const Module* PackageRegistry::findModule(const std::string &identifier) const {
        auto it = moduleMap.find(identifier);

        if (it == moduleMap.end()) {
            return nullptr;
        }

        return it->second;
    }
}
