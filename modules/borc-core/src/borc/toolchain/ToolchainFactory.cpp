
#include <borc/toolchain/ToolchainFactory.hpp>

#include <stdexcept>
#include <borc/toolchain/Compiler.hpp>
#include <borc/toolchain/Linker.hpp>

#include "ToolchainImpl.hpp"
#include "ServiceFactoryGCC.hpp"
#include "ServiceFactoryVC.hpp"

namespace borc {
    class ToolchainFactoryImpl : public ToolchainFactory {
    public:
        ToolchainFactoryImpl() {}

        virtual ~ToolchainFactoryImpl() {}

        virtual std::unique_ptr<Toolchain> createToolchain(const ToolchainFamily family) override {
            ServiceFactory *serviceFactory = nullptr;

            switch (family) {
                case ToolchainFamily::VC: 
                    serviceFactory = &serviceFactoryVC;
                    break;

                case ToolchainFamily::GCC:
                    serviceFactory = &serviceFactoryGCC;
                    break;
            }

            if (serviceFactory) {
                const std::vector<std::pair<SourceType, const Compiler*>> compilers = {
                    { cppSourceType, serviceFactory->getCompiler() }
                };

                return std::make_unique<ToolchainImpl>(compilers, serviceFactory->getLinker());
            } 

            throw std::runtime_error("Unsupported toolchain");
        }

    private:
        SourceType cppSourceType = {"*.cpp", "*.cxx", "*.c++", "*.cc"};

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
