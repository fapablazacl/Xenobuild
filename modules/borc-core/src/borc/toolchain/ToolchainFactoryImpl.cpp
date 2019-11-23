
#include "ToolchainFactoryImpl.hpp"

#include <stdexcept>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/services/ToolchainServiceImpl.hpp>

#include "ToolchainImpl.hpp"


namespace borc {
    ToolchainFactoryImpl::ToolchainFactoryImpl(const boost::filesystem::path &toolchainDefinitionPath) {
        this->toolchainDefinitionPath = toolchainDefinitionPath;

        FileServiceImpl fileService;
        ToolchainServiceImpl toolchainService(&fileService);

        // TODO: Add auto discovery
        const std::vector<std::string> toolchainIds = {
            "vc", "gcc"
        };

        for (const std::string &toolchainId : toolchainIds) {
            const auto toolchainPath = toolchainDefinitionPath / toolchainId;

            toolchainMap.insert({toolchainId, toolchainService.createToolchain(toolchainPath)});
        }
    }

    ToolchainFactoryImpl::~ToolchainFactoryImpl() {}

    Toolchain* ToolchainFactoryImpl::createToolchain(const std::string &toolchainId) {
        if (auto toolchainIt = toolchainMap.find(toolchainId); toolchainIt != toolchainMap.end()) {
            toolchainIt->second.get();
        } 
        
        throw std::runtime_error("Unknown toolchain: " + toolchainId);
    }
}
