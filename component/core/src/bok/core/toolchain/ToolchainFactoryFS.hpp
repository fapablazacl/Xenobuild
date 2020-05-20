
#ifndef __BOK_TOOLCHAIN_TOOLCHAINFACTORYFS_HPP__
#define __BOK_TOOLCHAIN_TOOLCHAINFACTORYFS_HPP__

#include "ToolchainFactory.hpp"

#include <memory>
#include <map>
#include <boost/optional/optional_fwd.hpp>
#include <boost/filesystem/path.hpp>

namespace bok {
    /**
     * @brief Toolchain Factory that loads the requested toolchain from the filesystem.
     */
    class ToolchainFactoryFS : public ToolchainFactory {
    public:
        explicit ToolchainFactoryFS(const boost::filesystem::path &definitionPath, boost::optional<boost::filesystem::path> installationPath);

        virtual ~ToolchainFactoryFS();

        virtual Toolchain* createToolchain(const std::string &toolchainId) override;

    private:
        std::vector<std::string> detectAvailableToolchainIds() const;

    private:
        boost::filesystem::path definitionPath;
        std::map<std::string, std::unique_ptr<Toolchain>> toolchainMap;
    };
}

#endif
