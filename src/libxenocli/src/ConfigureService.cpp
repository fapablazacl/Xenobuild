
#include <Xenobuild/ConfigureService.h>

#include <boost/optional/optional_io.hpp>
#include <Xenobuild/Common.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/Context.h>
#include <Xenobuild/core/Dependency.h>
#include <Xenobuild/core/DependencyManager.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageManager.h>
#include <Xenobuild/core/CMakeBuildSystem.h>


namespace Xenobuild {
    ConfigureService::ConfigureService(PackageManager &packageManager, DependencyManager &dependencyManager) 
        : packageManager{packageManager}, dependencyManager{dependencyManager} {}

    ConfigureService::~ConfigureService() {}

    void ConfigureService::configure(const Package &package, const Toolchain &toolchain, const Triplet &triplet) {
        const CMakeBuildType buildTypes[] = {
            CMakeBuildType::Release, CMakeBuildType::Debug
        };

        const std::vector<Dependency> dependencies = package.dependencies;
        
        // 
        for (const Dependency &dep : dependencies) {
            if (! dependencyManager.download(dep)) {
                throw std::runtime_error("Download command failed.");
            }

            for (const CMakeBuildType buildType : buildTypes) {
                if (! dependencyManager.configure(dep, toolchain, buildType, {})) {
                    throw std::runtime_error("Configure command failed.");
                }

                if (! dependencyManager.build(dep, toolchain, buildType)) {
                    throw std::runtime_error("Build command failed.");
                }
                
                if (! dependencyManager.install(dep, toolchain, buildType)) {
                    throw std::runtime_error("Install command failed.");
                }
            }
        }

        // 
        for (const CMakeBuildType buildType : buildTypes) {
            if (! packageManager.configure(package, toolchain, triplet, buildType, dependencyManager)) {
                throw std::runtime_error("Configure command failed.");
            }
        }
    }
}
