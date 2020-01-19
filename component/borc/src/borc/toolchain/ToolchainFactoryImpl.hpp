
#ifndef __BORC_TOOLCHAIN_TOOLCHAINFACTORYIMPL_HPP__
#define __BORC_TOOLCHAIN_TOOLCHAINFACTORYIMPL_HPP__

#include "ToolchainFactory.hpp"

#include <memory>
#include <map>
#include <boost/optional/optional_fwd.hpp>
#include <boost/filesystem/path.hpp>

namespace borc {
    class ToolchainFactoryImpl : public ToolchainFactory {
    public:
        explicit ToolchainFactoryImpl(const boost::filesystem::path &definitionPath, boost::optional<boost::filesystem::path> installationPath);

        virtual ~ToolchainFactoryImpl();

        virtual Toolchain* createToolchain(const std::string &toolchainId) override;

    private:
        std::vector<std::string> detectAvailableToolchainIds() const;

    private:
        boost::filesystem::path definitionPath;
        std::map<std::string, std::unique_ptr<Toolchain>> toolchainMap;
    };
}

#endif
