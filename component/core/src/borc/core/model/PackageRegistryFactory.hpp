
#ifndef __BOK_MODEL_PACKAGEREGISTRYFACTORY_HPP__
#define __BOK_MODEL_PACKAGEREGISTRYFACTORY_HPP__

#include <memory>
#include <boost/filesystem/path.hpp>

namespace bok {
    class PackageService;
    class PackageRegistry;
    class PackageRegistryFactory {
    public:
        std::unique_ptr<PackageRegistry> createPackageRegistry(PackageService *packageService, const boost::filesystem::path &packageRegistryPath) const;
    };
}

#endif
