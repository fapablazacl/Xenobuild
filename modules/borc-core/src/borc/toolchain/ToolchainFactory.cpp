
#include "ToolchainFactory.hpp"

#include <borc/core/Compiler.hpp>
#include <borc/core/Linker.hpp>
#include <borc/core/Toolchain.hpp>
#include <borc/core/ServiceFactoryGCC.hpp>
#include <borc/core/ServiceFactoryVC.hpp>

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

                return std::make_unique<Toolchain>(compilers, serviceFactory->getLinker());
            } else {
                return std::unique_ptr<Toolchain>();
            }
        }

    private:
        SourceType cppSourceType = {"*.cpp", "*.cxx", "*.c++", "*.cc"};

        ServiceFactoryVC serviceFactoryVC = ServiceFactoryVC {
            "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Tools\\MSVC\\14.16.27023\\",
            "C:\\Program Files (x86)\\Windows Kits\\10\\"
        };

        ServiceFactoryGCC serviceFactoryGCC = ServiceFactoryGCC {
            "/usr/local/Cellar/gcc/8.2.0/bin/gcc-8"
            // "gcc"
        };
    };

    std::unique_ptr<ToolchainFactory> ToolchainFactory::create() {
        return std::make_unique<ToolchainFactoryImpl>();
    }
}
