
#include <borc/services/PackageService.hpp>

#include <borc/model/Package.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/common/EntityLoaderFactory.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/LanguageEntity.hpp>
#include <borc/entity/ModuleEntity.hpp>
#include <borc/common/EntityLoader.hpp>
#include <borc/common/EntityLoaderFactory.hpp>
#include <borc/common/PackageFactory.hpp>

namespace borc {
    std::unique_ptr<Package> PackageService::createPackage(const boost::filesystem::path &packageBaseFolder) const {
        FileServiceImpl service;

        EntityLoaderFactory entityLoaderFactory;

        const auto entityLoader = entityLoaderFactory.createLoader(packageBaseFolder, service);
        const PackageEntity packageEntity = entityLoader->loadPackageEntity();
        const std::vector<ModuleEntity> moduleEntities = entityLoader->loadModuleEntities(packageEntity);
        
        PackageFactory packageFactory;
        std::unique_ptr<Package> package = packageFactory.createPackage(packageEntity, moduleEntities);

        return package;
    }
}
