
#include "ToolchainFactory.hpp"

#include <memory>
#include <boost/filesystem/path.hpp>

namespace bok {
    class ToolchainFactory_Mock : public ToolchainFactory {
    public:
        ToolchainFactory_Mock();

        virtual ~ToolchainFactory_Mock();

        Toolchain* getToolchain(const std::string &toolchainId) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
}
