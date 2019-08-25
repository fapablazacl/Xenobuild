
#include "ConfigureController.hpp"
#include "ConfigureControllerOptions.hpp"

namespace borc {
    ConfigureController::~ConfigureController() {}

    void ConfigureController::perform(int argc, char **argv) {
        const auto options = ConfigureControllerOptions::parse(argc, argv);

    }
}
