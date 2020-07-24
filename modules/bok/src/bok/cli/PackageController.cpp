
#include "PackageController.hpp"

#include <string>

namespace bok {
    enum class PackageAction {
        Clean,
        Build
    };

    struct PackageController::Options {
        PackageAction action;        
    };

    PackageController::Options PackageController::parse(int argc, char **argv) const {
        return { PackageAction::Build };
    }

    void PackageController::perform(int argc, char **argv) {
        const auto options = parse(argc, argv);

        switch (options.action) {
        case PackageAction::Build:
            
            break;

        case PackageAction::Clean:
            break;
        }
    }
}
