
#pragma once 

#include "Controller.h"

#include <string>

namespace Xenobuild {
    struct BuildControllerInput {
        std::string sourceDir;
        std::string buildDir;

        static BuildControllerInput parse(int argc, char** argv);
    };


    class BuildController : public Controller {
    public:
        BuildController(int argc, char **argv);

        void perform() override;

    private:
        BuildControllerInput input;
    };
}
