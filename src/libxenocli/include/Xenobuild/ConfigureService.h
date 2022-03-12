
#pragma once

#include <Xenobuild/core/Triplet.h>

#include <string>
#include <ostream>
#include <memory>


namespace Xenobuild {
    struct Context;
    
    class CommandExecutor;
    class PackageFactory;
    class PackageManager;
    class DependencyManager;
    class ConfigureService {
    public:
        ConfigureController(PackageManager &packageManager, DependencyManager &dependencyManager);

        virtual ~ConfigureController();

        void configure(const Package &package, const Toolchain &toolchain);

    private:
        PackageManager &packageManager;
        DependencyManager &dependencyManager;
    };
}
