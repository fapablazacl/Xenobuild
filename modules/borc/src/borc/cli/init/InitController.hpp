
#ifndef __BORC_CLI_INITCONTROLLER_HPP__
#define __BORC_CLI_INITCONTROLLER_HPP__

#include <borc/cli/ControllerStub.hpp>
#include "InitControllerOptions.hpp"

namespace borc {
    struct Version;

    /**
     * @brief Setups an initial project structure at the specified directory
     */
    class InitController : public ControllerStub<InitControllerOptions> {
    public:
        virtual ~InitController();

        virtual void perform(const InitControllerOptions &options) override;

    private:
        Version detectToolchainVersion() const;
    };
}

#endif
