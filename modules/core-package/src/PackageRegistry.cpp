
#include <bok/core/package/PackageRegistry.hpp>

#include <iostream>
#include <bok/core/package/Module.hpp>
#include <bok/core/package/Package.hpp>

namespace bok {
    PackageRegistry::PackageRegistry() {}

    PackageRegistry::~PackageRegistry() {}

    void PackageRegistry::registerPackage(std::unique_ptr<Package> package) {
        for (const Component *component : package->getModules()) {
            const std::string key = this->getModuleIdentifier(component);

            std::cout << "Registered " << key << " component." << std::endl;

            moduleMap.insert({key, component});
        }

        packages.push_back(std::move(package));
    }


    std::string PackageRegistry::getModuleIdentifier(const Component *component) const {
        return component->getPackage()->getName() + "/" + component->getName();
    }


    const Component* PackageRegistry::findModule(const std::string &identifier) const {
        auto it = moduleMap.find(identifier);

        if (it == moduleMap.end()) {
            std::cout << identifier << " component  not found" << std::endl;

            return nullptr;
        }

        std::cout << identifier << " component found!" << std::endl;

        return it->second;
    }
}
