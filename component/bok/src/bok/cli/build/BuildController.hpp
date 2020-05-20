
#ifndef __BOK_CLI_BUILDCONTROLLER_HPP__
#define __BOK_CLI_BUILDCONTROLLER_HPP__

#include <bok/cli/ControllerStub.hpp>
#include "BuildControllerOptions.hpp"

namespace bok {
    class BuildController : public ControllerStub<BuildControllerOptions> {
    public:
        virtual ~BuildController();

        virtual void perform(const BuildControllerOptions &options) override;
    };
}

#endif
