
#include "ConfigureController.hpp"
#include <iostream>

namespace borc {
    ConfigureController::~ConfigureController() {}

    void ConfigureController::perform(const ConfigureControllerOptions &options) {
        if (options.showHelp) {
            std::cout << options.helpMessage;

            return;
        }

        if (!options.buildType && !options.toolchain) {
            throw std::runtime_error("Must select a build type and a toolchain.");
        }

        std::cout << "Configuring build: type=" << options.buildType.get() << ", toolchain=" << options.toolchain.get() << std::endl;
    }
}
