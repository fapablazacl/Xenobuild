
#ifndef __BORC_SERVICES_TOOLCHAINSERVICEIMPL_HPP__
#define __BORC_SERVICES_TOOLCHAINSERVICEIMPL_HPP__

#include "ToolchainService.hpp"

namespace bok {
    class FileService;

    class ToolchainServiceImpl : public ToolchainService {
    public:
        explicit ToolchainServiceImpl(const FileService *fileService);

        std::unique_ptr<Toolchain> createToolchain(const boost::filesystem::path &toolchainFolder, boost::optional<boost::filesystem::path> installationPath) const override;

    private:
        const FileService *fileService = nullptr;
    }; 
}

#endif
