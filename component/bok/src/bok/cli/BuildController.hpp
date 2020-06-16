
#ifndef __BOK_CLI_BUILDCONTROLLER_HPP__
#define __BOK_CLI_BUILDCONTROLLER_HPP__

#include <bok/cli/Controller.hpp>

namespace bok {
    class BuildController : public Controller {
    public:
        virtual ~BuildController() {}

        virtual void perform(int argc, char **argv) override;
    };
}

#endif
