
#pragma once 

#include "Controller.h"

#include <string>

namespace Xenobuild {
    struct BuildControllerInput {
        std::string sourceDir;
        std::string buildDir;

        static BuildControllerInput parse(int argc, char** argv);
    };

    struct Context;

    class BuildController : public Controller {
    public:
        typedef BuildControllerInput Params;
        
        static const char* Name;

    public:
        BuildController(Context& context, const BuildControllerInput &params);

        void perform() override;

    private:
        Context& context;
        BuildControllerInput params;
    };
}
