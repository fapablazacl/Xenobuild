
#pragma once

namespace Xenobuild {
    struct Package;
    struct Triplet;

    class Toolchain;
    class PackageManager;
    class DependencyManager;
    class ConfigureService {
    public:
        ConfigureService(PackageManager &packageManager, DependencyManager &dependencyManager);

        virtual ~ConfigureService();

        void configure(const Package &package, const Toolchain &toolchain, const Triplet &triplet);

    private:
        PackageManager &packageManager;
        DependencyManager &dependencyManager;
    };
}
