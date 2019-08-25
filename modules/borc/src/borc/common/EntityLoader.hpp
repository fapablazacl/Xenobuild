
#ifndef __BORC_COMMON_ENTITYLOADER_HPP__
#define __BORC_COMMON_ENTITYLOADER_HPP__

#include <boost/filesystem/path.hpp>

namespace borc {
    class FileService;

    struct PackageEntity;
    struct ModuleEntity;

    class EntityLoader {
    public:
        EntityLoader(const boost::filesystem::path &basePath, FileService &fileService);

        PackageEntity loadPackageEntity() const;

        std::vector<ModuleEntity> loadModuleEntities(const PackageEntity &packageEntity) const;

        bool checkValidBorcFile(const boost::filesystem::path &filePath) const;

    private:
        boost::filesystem::path basePath;
        FileService &fileService;
    };
}

#endif
