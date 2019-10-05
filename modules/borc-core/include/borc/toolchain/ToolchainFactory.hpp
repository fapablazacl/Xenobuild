
#ifndef __BORC_CORE_TOOLCHAINFACTORY_HPP__
#define __BORC_CORE_TOOLCHAINFACTORY_HPP__

#include <memory>

namespace borc {
    class Toolchain;
    class ToolchainFactory {
    public:
        virtual ~ToolchainFactory() {}

        virtual std::unique_ptr<Toolchain> createToolchain(const std::string &toolchainId) = 0;

    public:
        static std::unique_ptr<ToolchainFactory> create();
    };
}

#endif
