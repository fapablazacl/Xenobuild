
#include <bok/core/toolchain/ToolchainFactory_Mock.hpp>

#include <map>
#include <vector>

#include <bok/core/toolchain/Toolchain_Mock.hpp>
#include <bok/core/toolchain/vc/Toolchain_VC.hpp>

namespace bok {
    struct ToolchainFactory_Mock::Impl {
        std::map<std::string, std::unique_ptr<Toolchain>> toolchainMap;
    };

    ToolchainFactory_Mock::ToolchainFactory_Mock() 
        : impl(new ToolchainFactory_Mock::Impl()) {

        impl->toolchainMap["mock"] = std::make_unique<Toolchain_Mock>();
        impl->toolchainMap["vc"] = std::make_unique<Toolchain_VC>("C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.22.27905");
    }

    ToolchainFactory_Mock::~ToolchainFactory_Mock() {}

    Toolchain* ToolchainFactory_Mock::getToolchain(const std::string &toolchainId) {
        Toolchain* toolchain = nullptr;

        if (auto it = impl->toolchainMap.find(toolchainId); it != impl->toolchainMap.end()) {
            toolchain = it->second.get();
        }

        return toolchain;
    }
}
