
#ifndef __BORC_COMMON_ENTITYLOADER_HPP__
#define __BORC_COMMON_ENTITYLOADER_HPP__

#include <vector>

namespace borc {
    struct PackageEntity;
    struct ModuleEntity;

    class EntityLoader {
    public:
        virtual ~EntityLoader();

        virtual PackageEntity loadPackageEntity() const = 0;

        virtual std::vector<ModuleEntity> loadModuleEntities(const PackageEntity &packageEntity) const = 0;
    };
}

#endif
