
#pragma once 

#include <bok/cli/Controller.hpp>

namespace bok {
    /**
     * @brief Implements all the operations / options supported by the 'package' object in the bok command line interface
     */
    class PackageController : public Controller {
    public:
        // PackageController();
        // virtual ~PackageController();

        void perform(int argc, char **argv) override;

    private:
        struct Options;

    private:
        Options parse(int argc, char **argv) const;

    private:

    };
}
