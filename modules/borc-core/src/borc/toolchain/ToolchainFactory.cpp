
#include "ToolchainFactory.hpp"

#include <stdexcept>

#include <borc/services/FileServiceImpl.hpp>
#include <borc/services/ToolchainServiceImpl.hpp>
#include <borc/toolchain/Compiler.hpp>
#include <borc/toolchain/Linker.hpp>
#include <borc/toolchain/SourceChecker.hpp>
#include <borc/toolchain/ModuleChecker.hpp>

#include "ToolchainImpl.hpp"
#include "ServiceFactoryGCC.hpp"
#include "ServiceFactoryVC.hpp"
#include "ModuleTypeChecker.hpp"

namespace borc {
    class ToolchainFactoryImpl : public ToolchainFactory {
    public:
        explicit ToolchainFactoryImpl(const boost::filesystem::path &toolchainDefinitionPath) {
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

        virtual ~ToolchainFactoryImpl() {}

        virtual Toolchain* createToolchain(const std::string &toolchainId) override {
            if (auto toolchainIt = toolchainMap.find(toolchainId); toolchainIt != toolchainMap.end()) {
                toolchainIt->second.get();
            } 
            
            throw std::runtime_error("Unknown toolchain: " + toolchainId);
        }

    private:
        boost::filesystem::path toolchainDefinitionPath;

        std::map<std::string, std::unique_ptr<Toolchain>> toolchainMap;

        ServiceFactoryVC serviceFactoryVC {
            "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Tools\\MSVC\\14.16.27023\\",
            "C:\\Program Files (x86)\\Windows Kits\\10\\"
        };

        ServiceFactoryGCC serviceFactoryGCC {
            // "/usr/local/Cellar/gcc/8.2.0/bin/gcc-8"  // For macOS
            "gcc"                                       // for other unixes
        };
    };

    std::unique_ptr<ToolchainFactory> ToolchainFactory::create(const boost::filesystem::path &toolchainDefinitionPath) {
        return std::make_unique<ToolchainFactoryImpl>(toolchainDefinitionPath);
    }
}
