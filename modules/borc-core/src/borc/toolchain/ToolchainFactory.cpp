
#include <borc/toolchain/ToolchainFactory.hpp>

#include <stdexcept>
#include <borc/toolchain/Compiler.hpp>
#include <borc/toolchain/Linker.hpp>
#include <borc/toolchain/SourceChecker.hpp>
#include <borc/toolchain/ArtifactChecker.hpp>

#include "ToolchainImpl.hpp"
#include "ServiceFactoryGCC.hpp"
#include "ServiceFactoryVC.hpp"
#include "ArtifactTypeChecker.hpp"

namespace borc {
    class ToolchainFactoryImpl : public ToolchainFactory {
    public:
        ToolchainFactoryImpl() {}

        virtual ~ToolchainFactoryImpl() {}

        virtual std::unique_ptr<Toolchain> createToolchain(const std::string &toolchainId) override {
            ServiceFactory *serviceFactory = nullptr;

            if (toolchainId == "gcc") {
                serviceFactory = &serviceFactoryGCC;
            } else if (toolchainId == "vc") {
                serviceFactory = &serviceFactoryVC;
            }

            if (! serviceFactory) {
                throw std::runtime_error("Unknown toolchain: " + toolchainId);
            }

            return std::make_unique<ToolchainImpl>(serviceFactory->getCompilers(), serviceFactory->getLinkers());
        }

    private:
        ServiceFactoryVC serviceFactoryVC {
            "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Tools\\MSVC\\14.16.27023\\",
            "C:\\Program Files (x86)\\Windows Kits\\10\\"
        };

        ServiceFactoryGCC serviceFactoryGCC {
            // "/usr/local/Cellar/gcc/8.2.0/bin/gcc-8"  // For macOS
            "gcc"                                       // for other unixes
        };
    };

    std::unique_ptr<ToolchainFactory> ToolchainFactory::create() {
        return std::make_unique<ToolchainFactoryImpl>();
    }
}
