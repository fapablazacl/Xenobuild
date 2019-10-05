
#include "ConfigureController.hpp"
#include <iostream>

#include <borc/toolchain/Toolchain.hpp>
#include <borc/toolchain/ToolchainFactory.hpp>

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

        auto factory = ToolchainFactory::create();
        auto toolchain = factory->createToolchain(options.toolchain.get());
    }
}
