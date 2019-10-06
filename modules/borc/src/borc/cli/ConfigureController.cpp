
#include "ConfigureController.hpp"
#include <iostream>

#include <boost/filesystem.hpp>
#include <borc/model/Version.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/toolchain/ToolchainFactory.hpp>
#include <borc/build/BuildCache.hpp>
#include <borc/build/BuildCacheFactory.hpp>

namespace borc {
    const std::string cxxDetectorSource = R"(
#include <stdio.h>

struct compiler_t {
    const char *name;
    int version_major;
    int version_minor;
    int version_patch;
};

int main() {
    struct compiler_t compiler = {
        NULL, 0, 0, 0
    };

#if defined(__clang__)
    compiler.name = "clang";
    compiler.version_major = __clang_major__;
    compiler.version_minor = __clang_minor__;
    compiler.version_patch = __clang_patchlevel__;
#elif defined(__GNUC__)
    compiler.name = "gcc";
    compiler.version_major = __GNUC__;
    compiler.version_minor = __GNUC_MINOR__;
    compiler.version_patch = __GNUC_PATCHLEVEL__;
#endif

    if (compiler.name) {
        printf("%s-%d.%d.%d\n", compiler.name, compiler.version_major, compiler.version_minor, compiler.version_patch);
        
        return 0;
    } else {
        return 1;
    }
})";

    ConfigureController::~ConfigureController() {}

    void ConfigureController::perform(const ConfigureControllerOptions &options) {
        if (options.showHelp) {
            std::cout << options.helpMessage;

            return;
        }

        // grab current configured builds and show them to the user.
        const auto packagePath = boost::filesystem::current_path();
        const auto outputPath = packagePath / ".borc";

        auto buildCacheFactory = BuildCacheFactory{};
        auto buildCache = buildCacheFactory.createBuildCache(outputPath);
        auto buildCacheData = buildCache->getData();

        if (buildCacheData.sourceSetMap.size() == 0 && !options.buildType && !options.toolchain) {
            throw std::runtime_error(
                "There is no configurations associated. Must select a build type and a toolchain.\n"
                "See 'borc configure --help' for details."
            );
        }

        if (!options.buildType && !options.toolchain) {
            std::cout << "Configured builds for current package:" << std::endl;

            for (const auto &pair : buildCacheData.sourceSetMap) {
                const auto &config = pair.first;
                std::cout << "    " << config.toolchainId << "-" << config.version << "(" << config.arch << ")" << std::endl;
            }

            return;
        }

        std::cout << "Configuring build: type=" << options.buildType.get() << ", toolchain=" << options.toolchain.get() << std::endl;

        auto factory = ToolchainFactory::create();
        auto toolchain = factory->createToolchain(options.toolchain.get());

        // detect compiler version
        Version toolchainVersion = this->detectToolchainVersion();

        // setup the configuration requested by the user
        auto config = BuildConfiguration{};
        config.toolchainId = options.toolchain.get();
        config.arch = "x86_64";
        config.type = options.buildType.get();
        config.version = toolchainVersion;

        buildCache->addBuildConfiguration(config);
    }

    Version ConfigureController::detectToolchainVersion() const {
        // 1. Compile C++ version detector

        // 2. Execute it, and grab the output

        // 3. Parse the output and return the result.
        return {1, 1, 1};
    }
}
