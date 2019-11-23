
#ifndef __BORC_CORE_TOOLCHAINFACTORY_HPP__
#define __BORC_CORE_TOOLCHAINFACTORY_HPP__

#include <memory>
#include <string>

#include <boost/filesystem/path.hpp>

namespace borc {
    class Toolchain;
    class ToolchainFactory {
    public:
        virtual ~ToolchainFactory() {}

        virtual Toolchain* createToolchain(const std::string &toolchainId) = 0;

    public:
        static std::unique_ptr<ToolchainFactory> create(const boost::filesystem::path &toolchainDefinitionPath);
    };
}

#endif
