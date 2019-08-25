
#ifndef __BORC_CLI_BUILDCONTROLLER_HPP__
#define __BORC_CLI_BUILDCONTROLLER_HPP__

#include "Controller.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/optional/optional.hpp>

namespace borc {
    struct PackageEntity;
    struct ModuleEntity;

    class Package;
    class FileService;

    class BuildController : public Controller {
    public:
        virtual ~BuildController();

        virtual void perform(int argc, char **argv) override;

    private:
        struct Options {
            boost::optional<std::string> buildType;
            boost::optional<std::string> toolchain;
            bool force = false;
            bool showHelp = false;
            std::string helpMessage = "";
        };

    private:
        Options parseOptions(int argc, char **argv) const;

        bool checkValidBorcFile(const boost::filesystem::path &filePath) const;

        PackageEntity makePackageEntity(const boost::filesystem::path &basePath, FileService &fileService) const;

        std::vector<ModuleEntity> makeModuleEntities(const boost::filesystem::path &basePath, FileService &fileService, const PackageEntity &packageEntity) const;

        std::unique_ptr<Package> makePackage(const PackageEntity &packageEntity, const std::vector<ModuleEntity> &moduleEntities) const;
    };
}

#endif
