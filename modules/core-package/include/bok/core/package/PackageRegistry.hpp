
#ifndef __BOK_MODEL_PACKAGEREGISTRY_HPP__
#define __BOK_MODEL_PACKAGEREGISTRY_HPP__

#include <map>
#include <vector>
#include <string>
#include <memory>

namespace bok {
    class Package;
    class Module;

    /**
     * @brief Contains a package registry, for storing all the stock packages offered by BORC.
     * @todo Add support for package and component versioning
     * 
     * Has all the supplemental data needed to build the current component, like the currently parsed and referenced software modules.
     */
    class PackageRegistry {
    public:
        PackageRegistry();

        ~PackageRegistry();

        void registerPackage(std::unique_ptr<Package> package);

        const Module* findModule(const std::string &identifier) const;

    private:
        std::string getModuleIdentifier(const Module *component) const;

    private:
        std::vector<std::unique_ptr<Package>> packages;
        std::map<std::string, const Module*> moduleMap;
    };
}

#endif
