
#pragma once 

#include "Controller.h"

#include <string>

namespace Xenobuild {
    struct SetupControllerInput {
        std::string sourceDir;
        std::string buildDir;

        static SetupControllerInput parse(int argc, char** argv);
    };

    class PackageFactory;
    class SetupController : public Controller {
    public:
        typedef SetupControllerInput Params;

    public:
        SetupController(PackageFactory &packageFactory, const SetupControllerInput &params);

        void perform() override;

    private:
        PackageFactory& packageFactory;
        SetupControllerInput params;
    };
}
