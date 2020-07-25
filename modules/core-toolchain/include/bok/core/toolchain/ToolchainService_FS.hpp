
#ifndef __BOK_SERVICES_TOOLCHAINSERVICEIMPL_HPP__
#define __BOK_SERVICES_TOOLCHAINSERVICEIMPL_HPP__

#include "ToolchainService.hpp"

namespace bok {
    class FileService;

    class ToolchainService_FS : public ToolchainService {
    public:
        explicit ToolchainService_FS(const FileService *fileService);

        std::unique_ptr<Toolchain> createToolchain(const boost::filesystem::path &toolchainFolder, boost::optional<boost::filesystem::path> installationPath) const override;

    private:
        const FileService *fileService = nullptr;
    }; 
}

#endif
