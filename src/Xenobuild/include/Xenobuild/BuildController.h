
#pragma once 

#include "Controller.h"

#include <string>

namespace Xenobuild {
    struct BuildControllerInput {
        std::string sourceDir;
        std::string buildDir;

        static BuildControllerInput parse(int argc, char** argv);
    };

    class PackageFactory;
    class BuildController : public Controller {
    public:
        typedef BuildControllerInput Params;

    public:
        BuildController(PackageFactory &packageFactory, const BuildControllerInput &params);

        void perform() override;

    private:
        PackageFactory& packageFactory;
        BuildControllerInput params;
    };
}
