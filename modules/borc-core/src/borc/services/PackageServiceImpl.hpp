
#ifndef __BORC_SERVICES_PACKAGESERVICEIMPL_HPP__
#define __BORC_SERVICES_PACKAGESERVICEIMPL_HPP__

#include "PackageService.hpp"

#include <vector>

namespace borc {
    struct PackageEntity;
    struct ModuleEntity;

    class PackageServiceImpl : public PackageService {
    public:
        virtual std::unique_ptr<Package> createPackage(const boost::filesystem::path &packageFilePath) const override;

    private:
        std::unique_ptr<Package> createPackageImpl(const PackageEntity &packageEntity, const std::vector<ModuleEntity> &moduleEntities) const;
    }; 
}

#endif
