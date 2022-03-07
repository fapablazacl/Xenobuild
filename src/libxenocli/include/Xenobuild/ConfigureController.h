
#pragma once

#include "Controller.h"
#include <Xenobuild/core/Triplet.h>

#include <string>
#include <ostream>
#include <memory>


namespace Xenobuild {
    struct Context;

    struct ConfigureControllerInput {
        std::string sourceDir;
        std::string buildDir;
        Triplet triplet;
        bool showEnvironment = true;

        static ConfigureControllerInput parse(const std::vector<std::string> &args);
    };
    
    inline std::ostream& operator<< (std::ostream &os, const ConfigureControllerInput &input) {
        os << "ConfigureControllerInput { ";
        os << input.sourceDir << ", ";
        os << input.buildDir << ", ";
        os << input.triplet << ", ";
        os << input.showEnvironment << "}";
        
        return os;
    }
    
    class CommandExecutor;
    class PackageFactory;
    class ConfigureController : public Controller {
    public:
        typedef ConfigureControllerInput Params;
        
        static const char* Name;

    public:
        ConfigureController(Context &context, const ConfigureControllerInput &params);

        ConfigureController(Context &context, const ConfigureControllerInput &params, std::unique_ptr<CommandExecutor> executor);

        virtual ~ConfigureController();

        void perform() override;

    private:
        Context &context;
        ConfigureControllerInput params;
        std::unique_ptr<CommandExecutor> executor;
    };
}
