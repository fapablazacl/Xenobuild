
#include "PackageRegistry.hpp"

#include <iostream>
#include <bok/model/Module.hpp>
#include <bok/model/Package.hpp>

namespace bok {
    PackageRegistry::PackageRegistry() {}

    PackageRegistry::~PackageRegistry() {}

    void PackageRegistry::registerPackage(std::unique_ptr<Package> package) {
        for (const Module *module : package->getModules()) {
            const std::string key = this->getModuleIdentifier(module);

            std::cout << "Registered " << key << " module." << std::endl;

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
            std::cout << identifier << " module  not found" << std::endl;

            return nullptr;
        }

        std::cout << identifier << " module found!" << std::endl;

        return it->second;
    }
}
