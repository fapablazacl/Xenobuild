
#ifndef __BORC_COMMON_PACKAGEFACTORY_HPP__
#define __BORC_COMMON_PACKAGEFACTORY_HPP__

#include <memory>
#include <vector>

namespace borc {
    class Package;

    struct PackageEntity;
    struct ModuleEntity;

    class PackageFactory {
    public:
        std::unique_ptr<Package> createPackage(const PackageEntity &packageEntity, const std::vector<ModuleEntity> &moduleEntities);
    };
}

#endif
