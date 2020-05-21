
#ifndef __BOK_CLI_CONTROLLERSTUB_HPP__
#define __BOK_CLI_CONTROLLERSTUB_HPP__

#include "Controller.hpp"

namespace bok {
    template<typename ControllerOptions>
    class ControllerStub : public Controller {
    public:
        virtual ~ControllerStub() {}

        virtual void perform(int argc, char **argv) override {
            auto options = ControllerOptions::parse(argc, argv);

            this->perform(options);
        }

    protected:
        virtual void perform(const ControllerOptions &options) = 0;
    };
}

#endif
