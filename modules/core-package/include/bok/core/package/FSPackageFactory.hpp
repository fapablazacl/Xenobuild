
#ifndef __BOK_SERVICES_PACKAGESERVICEIMPL_HPP__
#define __BOK_SERVICES_PACKAGESERVICEIMPL_HPP__

#include "PackageFactory.hpp"

#include <vector>

namespace bok {
    struct PackageEntity;
    struct ComponentEntity;

    class FileService;

    class FSPackageFactory : public PackageFactory {
    public:
        FSPackageFactory(const FileService *fileService);

        virtual std::unique_ptr<Package> createPackage(const boost::filesystem::path &packageFilePath, const PackageRegistry *packageRegistry) const override;

    private:
        std::unique_ptr<Package> createPackageImpl(const PackageEntity &packageEntity, const std::vector<ComponentEntity> &moduleEntities, const PackageRegistry *packageRegistry) const;

        PackageEntity loadPackageEntity(const boost::filesystem::path &packagePath) const;

        std::vector<ComponentEntity> loadModuleEntities(const boost::filesystem::path &packagePath, const PackageEntity &packageEntity) const;

        bool checkValidBorcFile(const boost::filesystem::path &filePath) const;


    private:
        const FileService *fileService = nullptr;
    }; 
}

#endif
