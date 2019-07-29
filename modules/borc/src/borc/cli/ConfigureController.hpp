
#ifndef __BORC_CLI_CONFIGURECONTROLLER_HPP__
#define __BORC_CLI_CONFIGURECONTROLLER_HPP__

#include "Controller.hpp"

#include <boost/filesystem/path.hpp>

namespace borc {
    struct PackageEntity;
    struct ModuleEntity;

    class Package;
    class FileService;

    class ConfigureController : public Controller {
    public:
        virtual ~ConfigureController();

        virtual void perform(int argc, char **argv) override;

    private:
        bool checkValidBorcFile(const boost::filesystem::path &filePath) const;

        PackageEntity makePackageEntity(const boost::filesystem::path &basePath, FileService &fileService) const;

        std::vector<ModuleEntity> makeModuleEntities(const boost::filesystem::path &basePath, FileService &fileService, const PackageEntity &packageEntity) const;

        std::unique_ptr<Package> makePackage(const PackageEntity &packageEntity, const std::vector<ModuleEntity> &moduleEntities) const;
    };
}

#endif
