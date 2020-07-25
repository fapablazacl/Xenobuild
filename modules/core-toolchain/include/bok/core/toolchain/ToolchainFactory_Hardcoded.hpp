
#include "ToolchainFactory.hpp"

#include <memory>
#include <boost/filesystem/path.hpp>

namespace bok {
    class ToolchainFactory_Hardcoded : public ToolchainFactory {
    public:
        ToolchainFactory_Hardcoded();

        virtual ~ToolchainFactory_Hardcoded();

        void setInstallationPath(const std::string &toolchainId, const boost::filesystem::path &path);

        Toolchain* createToolchain(const std::string &toolchainId) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
}
