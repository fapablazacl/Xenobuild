
#include <bok/core/toolchain/ToolchainFactory_FS.hpp>

#include <stdexcept>

#include <bok/core/Constants.hpp>
#include <bok/core/FileService_FS.hpp>
#include <bok/core/toolchain/ToolchainService_FS.hpp>

namespace bok {
    ToolchainFactory_FS::ToolchainFactory_FS(const boost::filesystem::path &definitionPath, boost::optional<boost::filesystem::path> installationPath) {
        this->definitionPath = definitionPath;

        FileService_FS fileService;
        ToolchainService_FS toolchainService(&fileService);

        // TODO: Add auto discovery via reading the bok.json
        const std::vector<std::string> toolchainIds = this->detectAvailableToolchainIds();

        for (const std::string &toolchainId : toolchainIds) {
            const auto definitionFullPath = BOK_TOOLCHAIN_PATH_PREFIX / definitionPath / toolchainId;

            // TODO: Each toolchain should have a different installation path
            toolchainMap.insert({toolchainId, toolchainService.createToolchain(definitionFullPath, installationPath)});
        }
    }


    ToolchainFactory_FS::~ToolchainFactory_FS() {}


    Toolchain* ToolchainFactory_FS::createToolchain(const std::string &toolchainId) {
        if (auto toolchainIt = toolchainMap.find(toolchainId); toolchainIt != toolchainMap.end()) {
            return toolchainIt->second.get();
        }  

        throw std::runtime_error("Unknown toolchain: " + toolchainId);
    }


    std::vector<std::string> ToolchainFactory_FS::detectAvailableToolchainIds() const {
        return { "vc", "gcc" };
    }
}
