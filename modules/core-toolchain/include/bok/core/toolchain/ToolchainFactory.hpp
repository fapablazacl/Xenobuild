
#ifndef __BOK_CORE_TOOLCHAINFACTORY_HPP__
#define __BOK_CORE_TOOLCHAINFACTORY_HPP__

#include <string>

namespace bok {
    class Toolchain;
    class ToolchainFactory {
    public:
        virtual ~ToolchainFactory();

        virtual Toolchain* getToolchain(const std::string &toolchainId) = 0;
    };
}

#endif
