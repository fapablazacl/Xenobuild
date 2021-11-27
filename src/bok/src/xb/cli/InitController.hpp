
#ifndef __BOK_CLI_INITCONTROLLER_HPP__
#define __BOK_CLI_INITCONTROLLER_HPP__

#include <bok/cli/Controller.hpp>

namespace bok {
    struct Version;

    /**
     * @brief Setups an initial project structure at the specified directory
     */
    class InitController : public Controller {
    public:
        virtual ~InitController();

        virtual void perform(int argc, char **argv) override;
    };
}

#endif
