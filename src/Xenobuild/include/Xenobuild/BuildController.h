
#pragma once 

#include "Controller.h"

#include <string>
#include <vector>

namespace Xenobuild {
    struct BuildControllerInput {
        std::string sourceDir;
        std::string buildDir;

        static BuildControllerInput parse(const std::vector<std::string> &args);
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
