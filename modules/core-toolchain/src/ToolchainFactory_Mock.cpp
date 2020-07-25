
#include <bok/core/toolchain/ToolchainFactory_Mock.hpp>

#include <map>
#include <vector>

#include <bok/core/toolchain/Toolchain_Mock.hpp>

namespace bok {
    struct ToolchainFactory_Mock::Impl {
        Toolchain_Mock toolchain;
    };

    ToolchainFactory_Mock::ToolchainFactory_Mock() 
        : impl(new ToolchainFactory_Mock::Impl()) {}

    ToolchainFactory_Mock::~ToolchainFactory_Mock() {}

    Toolchain* ToolchainFactory_Mock::getToolchain(const std::string &toolchainId) {
        Toolchain* toolchain = nullptr;

        if (toolchainId == "mock") {
            toolchain = &impl->toolchain;
        }

        return toolchain;
    }
}
