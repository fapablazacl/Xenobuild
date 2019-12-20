
#include "ToolchainFactoryImpl.hpp"

#include <stdexcept>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/services/ToolchainServiceImpl.hpp>

#include "ToolchainImpl.hpp"


namespace borc {
    static const std::string TOOLCHAIN_PATH_PREFIX = "./etc/borc";


    ToolchainFactoryImpl::ToolchainFactoryImpl(const boost::filesystem::path &toolchainDefinitionPath) {
        this->toolchainDefinitionPath = toolchainDefinitionPath;

        FileServiceImpl fileService;
        ToolchainServiceImpl toolchainService(&fileService);

        // TODO: Add auto discovery via reading the borc.json
        const std::vector<std::string> toolchainIds = this->detectAvailableToolchainIds();

        for (const std::string &toolchainId : toolchainIds) {
            const auto toolchainPath = TOOLCHAIN_PATH_PREFIX / toolchainDefinitionPath / toolchainId;

            toolchainMap.insert({toolchainId, toolchainService.createToolchain(toolchainPath)});
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
