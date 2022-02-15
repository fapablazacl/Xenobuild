
#pragma once 

#include "Controller.h"

#include <Xenobuild/core/Triplet.h>
#include <string>

namespace Xenobuild {
    struct Context;

    struct SetupControllerInput {
        //! current source code folder
        std::string sourceDir;
        
        //! destination build folder
        std::string buildDir;
        
        //! Shows the current environment for the running process
        bool showEnvironment = true;

        //! the requested triplet to build for.
        Triplet triplet;

        static SetupControllerInput parse(int argc, char** argv);
    };

    class SetupController : public Controller {
    public:
        typedef SetupControllerInput Params;
        
        static const char* Name;

    public:
        SetupController(Context& context, const SetupControllerInput &params);

        void perform() override;

    private:
        Context& context;
        SetupControllerInput params;
    };
}
