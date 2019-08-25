
#ifndef __BORC_CLI_CONFIGURECONTROLLER_HPP__
#define __BORC_CLI_CONFIGURECONTROLLER_HPP__

#include "ControllerStub.hpp"
#include "ConfigureControllerOptions.hpp"

namespace borc {
    class ConfigureController : public ControllerStub<ConfigureControllerOptions> {
    public:
        virtual ~ConfigureController();

        virtual void perform(const ConfigureControllerOptions &options) override;
    };
}

#endif
