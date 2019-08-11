
#include "ConfigureController.hpp"

namespace borc {
    ConfigureController::~ConfigureController() {}

    void ConfigureController::perform(int argc, char **argv) {
        const auto options = this->parseCommandLine(argc, argv);

    }

    ConfigureController::Options ConfigureController::parseCommandLine(int arhc, char **argv) const {
        return {};
    }
}
