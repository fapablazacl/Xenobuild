
#ifndef __BORC_CORE_TOOLCHAINFACTORY_HPP__
#define __BORC_CORE_TOOLCHAINFACTORY_HPP__

#include <memory>

namespace borc {
    enum class ToolchainFamily { GCC, VC };

    class Toolchain;
    class ToolchainFactory {
    public:
        virtual ~ToolchainFactory() {}

        virtual std::unique_ptr<Toolchain> createToolchain(const ToolchainFamily family) = 0;

    public:
        static std::unique_ptr<ToolchainFactory> create();
    };
}

#endif
