
#ifndef __BOK_CLI_INITCONTROLLER_HPP__
#define __BOK_CLI_INITCONTROLLER_HPP__

#include <bok/cli/ControllerStub.hpp>
#include "InitControllerOptions.hpp"

namespace bok {
    struct Version;

    /**
     * @brief Setups an initial project structure at the specified directory
     */
    class InitController : public ControllerStub<InitControllerOptions> {
    public:
        virtual ~InitController();

        virtual void perform(const InitControllerOptions &options) override;
    };
}

#endif
