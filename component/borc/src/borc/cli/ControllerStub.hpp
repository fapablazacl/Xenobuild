
#ifndef __BORC_CLI_CONTROLLERSTUB_HPP__
#define __BORC_CLI_CONTROLLERSTUB_HPP__

#include "Controller.hpp"

namespace borc {
    template<typename ControllerOptions>
    class ControllerStub : public Controller {
    public:
        virtual ~ControllerStub() {}

        virtual void perform(int argc, char **argv) override {
            auto options = ControllerOptions::parse(argc, argv);

            this->perform(options);
        }

        virtual void perform(const ControllerOptions &options) = 0;
    };
}

#endif
