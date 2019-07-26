
#ifndef __BORC_CLI_CONFIGURECONTROLLER_HPP__
#define __BORC_CLI_CONFIGURECONTROLLER_HPP__

#include "Controller.hpp"

namespace borc {
    class ConfigureController : public Controller {
    public:
        virtual ~ConfigureController();

        virtual void perform(int argc, char **argv) override;
    };
}

#endif
