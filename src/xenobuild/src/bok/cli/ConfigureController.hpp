
#ifndef __BOK_CLI_CONFIGURECONTROLLER_HPP__
#define __BOK_CLI_CONFIGURECONTROLLER_HPP__

#include <bok/cli/Controller.hpp>

namespace bok {
    class ConfigureController : public Controller {
    public:
        ConfigureController();

        virtual ~ConfigureController();

        virtual void perform(int argc, char **argv) override;

    private:
        struct Private;
        Private *impl = nullptr;
    };
}

#endif
