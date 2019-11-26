
#ifndef __BORC_MODEL_CONTEXT_HPP__
#define __BORC_MODEL_CONTEXT_HPP__

#include <map>
#include <vector>
#include <string>

namespace borc {
    class Package;
    class Module;

    /**
     * @brief The execution context of the build system. 
     * @todo Add support for package and module versioning
     * 
     * Has all the supplemental data needed to build the current module, like the currently parsed and referenced software modules.
     */
    class PackageRegistry {
    public:
        PackageRegistry();

        ~PackageRegistry();

        void registerPackage(const Package *package);

        void unregisterPackage(const Package *package);

        const Module* findModule(const std::string &identifier) const;

    private:
        std::string getModuleIdentifier(const Module *module) const;

    private:
        std::vector<const Package*> packages;
        std::map<std::string, const Module*> moduleMap;
    };
}

#endif
