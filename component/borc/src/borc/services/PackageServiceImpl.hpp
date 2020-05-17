
#ifndef __BORC_SERVICES_PACKAGESERVICEIMPL_HPP__
#define __BORC_SERVICES_PACKAGESERVICEIMPL_HPP__

#include "PackageService.hpp"

#include <vector>

namespace bok {
    struct PackageEntity;
    struct ModuleEntity;

    class FileService;

    class PackageServiceImpl : public PackageService {
    public:
        PackageServiceImpl(const FileService *fileService);

        virtual std::unique_ptr<Package> createPackage(const boost::filesystem::path &packageFilePath, const PackageRegistry *packageRegistry) const override;

    private:
        std::unique_ptr<Package> createPackageImpl(const PackageEntity &packageEntity, const std::vector<ModuleEntity> &moduleEntities, const PackageRegistry *packageRegistry) const;

        PackageEntity loadPackageEntity(const boost::filesystem::path &packagePath) const;

        std::vector<ModuleEntity> loadModuleEntities(const boost::filesystem::path &packagePath, const PackageEntity &packageEntity) const;

        bool checkValidBorcFile(const boost::filesystem::path &filePath) const;


    private:
        const FileService *fileService = nullptr;
    }; 
}

#endif
