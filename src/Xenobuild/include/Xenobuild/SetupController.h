
#pragma once 

#include "Controller.h"

#include <string>

namespace Xenobuild {
    enum class Arch {
        X32,
        X64
    };

    enum class Platform {
        Windows,
        Linux,
        MacOS
    };

    enum class Toolchain {
        Clang,
        MicrosoftVC,
        AppleClang,
        GnuGCC
    };
    
    struct Triplet {
        Platform platform;
        Arch arch;
        Toolchain toolchain;
    };


    struct SetupControllerInput {
        std::string sourceDir;
        std::string buildDir;

        //! the requested triplet to build for.
        Triplet triplet;

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
