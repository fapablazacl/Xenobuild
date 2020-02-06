
#include "ToolchainFactoryImpl.hpp"

#include <stdexcept>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/services/ToolchainServiceImpl.hpp>

#include "ToolchainImpl.hpp"


namespace borc {
    static const std::string TOOLCHAIN_PATH_PREFIX = "./etc/borc";


    ToolchainFactoryImpl::ToolchainFactoryImpl(const boost::filesystem::path &definitionPath, boost::optional<boost::filesystem::path> installationPath) {
        this->definitionPath = definitionPath;

        FileServiceImpl fileService;
        ToolchainServiceImpl toolchainService(&fileService);

        // TODO: Add auto discovery via reading the borc.json
        const std::vector<std::string> toolchainIds = this->detectAvailableToolchainIds();

        for (const std::string &toolchainId : toolchainIds) {
            const auto definitionFullPath = TOOLCHAIN_PATH_PREFIX / definitionPath / toolchainId;

            // TODO: Each toolchain should have a different installation path
            toolchainMap.insert({toolchainId, toolchainService.createToolchain(definitionFullPath, installationPath)});
        }
    }


    ToolchainFactoryImpl::~ToolchainFactoryImpl() {}


    Toolchain* ToolchainFactoryImpl::createToolchain(const std::string &toolchainId) {
        if (auto toolchainIt = toolchainMap.find(toolchainId); toolchainIt != toolchainMap.end()) {
            return toolchainIt->second.get();
        }  

        throw std::runtime_error("Unknown toolchain: " + toolchainId);
    }


    std::vector<std::string> ToolchainFactoryImpl::detectAvailableToolchainIds() const {
        return { "vc", "gcc" };
    }
}