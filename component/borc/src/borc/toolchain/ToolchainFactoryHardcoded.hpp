
#include "ToolchainFactory.hpp"

#include <memory>
#include <boost/filesystem/path.hpp>

namespace bok {
    class ToolchainFactoryHardcoded : public ToolchainFactory {
    public:
        ToolchainFactoryHardcoded();

        virtual ~ToolchainFactoryHardcoded();

        void setInstallationPath(const std::string &toolchainId, const boost::filesystem::path &path);

        Toolchain* createToolchain(const std::string &toolchainId) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
}
