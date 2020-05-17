
#ifndef __BORC_CORE_TOOLCHAINFACTORY_HPP__
#define __BORC_CORE_TOOLCHAINFACTORY_HPP__

#include <string>

namespace bok {
    class Toolchain;
    class ToolchainFactory {
    public:
        virtual ~ToolchainFactory();

        virtual Toolchain* createToolchain(const std::string &toolchainId) = 0;
    };
}

#endif
