
#ifndef __BORC_CLI_CONFIGURECONTROLLER_HPP__
#define __BORC_CLI_CONFIGURECONTROLLER_HPP__

#include <set>
#include <borc/cli/ControllerStub.hpp>
#include "ConfigureControllerOptions.hpp"

namespace borc {

    struct Version;

    struct BuildType;
    class Toolchain;

    class PackageRegistry;
    class PackageService;

    class ConfigureController : public ControllerStub<ConfigureControllerOptions> {
    public:
        ConfigureController();

        virtual ~ConfigureController();

        virtual void perform(const ConfigureControllerOptions &options) override;

    private:
        struct Private;
        Private *m_impl = nullptr;
    };
}

#endif
