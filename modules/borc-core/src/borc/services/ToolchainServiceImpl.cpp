
#include "ToolchainServiceImpl.hpp"

#include <borc/common/EntityLoader.hpp>
#include <borc/common/EntityLoaderFactory.hpp>
#include <borc/model/Package.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/services/FileService.hpp>
#include <borc/entity/ToolchainEntity.hpp>

namespace borc {
    ToolchainServiceImpl::ToolchainServiceImpl(FileService *fileService) {
        this->fileService = fileService;
    }


    std::unique_ptr<Toolchain> ToolchainServiceImpl::createToolchain(const boost::filesystem::path &toolchainFolder) const {
        /*
        const auto entityLoader = entityLoaderFactory.createLoader(packageBaseFolder, service);
        const PackageEntity packageEntity = entityLoader->loadPackageEntity();
        const std::vector<ModuleEntity> moduleEntities = entityLoader->loadModuleEntities(packageEntity);
        
        PackageFactory packageFactory;

        auto toolchain  = packageFactory.createPackage(packageEntity, moduleEntities);

        return toolchain;
        */
    }
}
