
#ifndef __BORC_COMMON_JSONENTITYLOADER_HPP__
#define __BORC_COMMON_JSONENTITYLOADER_HPP__

#include <boost/filesystem/path.hpp>

#include "EntityLoader.hpp"

namespace borc {
    class FileService;

    struct PackageEntity;
    struct ModuleEntity;

    class JSONEntityLoader : public EntityLoader {
    public:
        JSONEntityLoader(const boost::filesystem::path &basePath, FileService &fileService);

        virtual ~JSONEntityLoader();

        virtual PackageEntity loadPackageEntity() const override;

        virtual std::vector<ModuleEntity> loadModuleEntities(const PackageEntity &packageEntity) const override;

    private:
        bool checkValidBorcFile(const boost::filesystem::path &filePath) const;

    private:
        boost::filesystem::path basePath;
        FileService &fileService;
    };
}

#endif
