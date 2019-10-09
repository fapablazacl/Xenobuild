
#ifndef __BORC_CLI_BUILDCONTROLLER_HPP__
#define __BORC_CLI_BUILDCONTROLLER_HPP__

#include <borc/cli/ControllerStub.hpp>
#include "BuildControllerOptions.hpp"

namespace borc {
    class BuildController : public ControllerStub<BuildControllerOptions> {
    public:
        virtual ~BuildController();

        virtual void perform(const BuildControllerOptions &options) override;
    };
}

#endif
