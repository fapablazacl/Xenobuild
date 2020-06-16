
#ifndef __BOK_CLI_CONFIGURECONTROLLER_HPP__
#define __BOK_CLI_CONFIGURECONTROLLER_HPP__

#include <set>
#include <bok/cli/Controller.hpp>
#include "ConfigureControllerOptions.hpp"

namespace bok {
    struct Version;

    struct BuildType;
    class Toolchain;

    class PackageRegistry;
    class PackageFactory;

    class ConfigureController : public Controller {
    public:
        ConfigureController();

        virtual ~ConfigureController();

        virtual void perform(int argc, char **argv) override;

    private:
        struct Private;
        Private *m_impl = nullptr;
    };
}

#endif
