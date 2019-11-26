
#ifndef __BORC_TOOLCHAIN_TOOLCHAINFACTORYIMPL_HPP__
#define __BORC_TOOLCHAIN_TOOLCHAINFACTORYIMPL_HPP__

#include "ToolchainFactory.hpp"

#include <memory>
#include <map>
#include <boost/filesystem/path.hpp>

namespace borc {
    class ToolchainFactoryImpl : public ToolchainFactory {
    public:
        explicit ToolchainFactoryImpl(const boost::filesystem::path &toolchainDefinitionPath);

        virtual ~ToolchainFactoryImpl();

        virtual Toolchain* createToolchain(const std::string &toolchainId) override;

    private:
        boost::filesystem::path toolchainDefinitionPath;
        std::map<std::string, std::unique_ptr<Toolchain>> toolchainMap;
    };
}


#endif