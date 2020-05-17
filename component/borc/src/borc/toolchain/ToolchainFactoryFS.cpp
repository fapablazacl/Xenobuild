
#include "ToolchainFactoryFS.hpp"

#include <stdexcept>

#include <borc/common/Constants.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/services/ToolchainServiceImpl.hpp>

#include "ToolchainImpl.hpp"

namespace borc {
    ToolchainFactoryFS::ToolchainFactoryFS(const boost::filesystem::path &definitionPath, boost::optional<boost::filesystem::path> installationPath) {
        this->definitionPath = definitionPath;

        FileServiceImpl fileService;
        ToolchainServiceImpl toolchainService(&fileService);

        // TODO: Add auto discovery via reading the borc.json
        const std::vector<std::string> toolchainIds = this->detectAvailableToolchainIds();

        for (const std::string &toolchainId : toolchainIds) {
            const auto definitionFullPath = BORC_TOOLCHAIN_PATH_PREFIX / definitionPath / toolchainId;

            // TODO: Each toolchain should have a different installation path
            toolchainMap.insert({toolchainId, toolchainService.createToolchain(definitionFullPath, installationPath)});
        }
    }


    ToolchainFactoryFS::~ToolchainFactoryFS() {}


    Toolchain* ToolchainFactoryFS::createToolchain(const std::string &toolchainId) {
        if (auto toolchainIt = toolchainMap.find(toolchainId); toolchainIt != toolchainMap.end()) {
            return toolchainIt->second.get();
        }  

        throw std::runtime_error("Unknown toolchain: " + toolchainId);
    }


    std::vector<std::string> ToolchainFactoryFS::detectAvailableToolchainIds() const {
        return { "vc", "gcc" };
    }
}
