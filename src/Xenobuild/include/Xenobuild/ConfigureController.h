
#pragma once

#include "Controller.h"
#include <Xenobuild/core/Triplet.h>

#include <string>
#include <ostream>

namespace Xenobuild {
    struct Context;

    struct ConfigureControllerInput {
        std::string sourceDir;
        std::string buildDir;
        Triplet triplet;
        bool showEnvironment = true;

        static ConfigureControllerInput parse(int argc, char** argv);
    };
    
    inline std::ostream& operator<< (std::ostream &os, const ConfigureControllerInput &input) {
        os << "ConfigureControllerInput { ";
        os << input.sourceDir << ", ";
        os << input.buildDir << ", ";
        os << input.triplet << ", ";
        os << input.showEnvironment << "}";
        
        return os;
    }
    
    class PackageFactory;
    class ConfigureController : public Controller {
    public:
        typedef ConfigureControllerInput Params;
        
        static const char* Name;

    public:
        ConfigureController(Context &context, const ConfigureControllerInput &params);

        void perform() override;

    private:
        Context &context;
        ConfigureControllerInput params;
    };
}
