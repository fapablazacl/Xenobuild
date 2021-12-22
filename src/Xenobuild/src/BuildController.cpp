
#include <Xenobuild/BuildController.h>
#include <iostream>

namespace Xenobuild {
    BuildController::BuildController(int argc, char **argv) {}

    void BuildController::perform() {
        std::cout << "BuildController::perform" << std::endl;
    }
}
