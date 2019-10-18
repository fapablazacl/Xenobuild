
#ifndef __BORC_MODEL_CONTEXT_HPP__
#define __BORC_MODEL_CONTEXT_HPP__

#include <map>
#include <vector>
#include <string>

namespace borc {
    class Package;
    class Artifact;

    /**
     * @brief The execution context of the build system. 
     * @todo Add support for package and module versioning
     * 
     * Has all the supplemental data needed to build the current artifact, like the currently parsed and referenced software modules.
     */
    class Context {
    public:
        Context();

        ~Context();

        void registerPackage(const Package *package);

        void unregisterPackage(const Package *package);

        const Artifact* findModule(const std::string &identifier) const;

    private:
        std::vector<const Package*> packages;
        std::map<std::string, const Artifact*> moduleMap;
    };
}

#endif
