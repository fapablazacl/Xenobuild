
#ifndef __BORC_CLI_CONFIGURECONTROLLER_HPP__
#define __BORC_CLI_CONFIGURECONTROLLER_HPP__

#include <borc/cli/ControllerStub.hpp>
#include "ConfigureControllerOptions.hpp"

namespace borc {

    struct Version;

    class ConfigureController : public ControllerStub<ConfigureControllerOptions> {
    public:
        virtual ~ConfigureController();

        virtual void perform(const ConfigureControllerOptions &options) override;

    private:
        Version detectToolchainVersion() const;
    };
}

#endif
