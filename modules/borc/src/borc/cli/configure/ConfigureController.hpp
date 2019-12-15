
#ifndef __BORC_CLI_CONFIGURECONTROLLER_HPP__
#define __BORC_CLI_CONFIGURECONTROLLER_HPP__

#include <set>
#include <borc/cli/ControllerStub.hpp>
#include "ConfigureControllerOptions.hpp"

namespace borc {

    struct Version;

    struct BuildType;
    class Toolchain;

    class PackageRegistry;
    class PackageService;

    class ConfigureController : public ControllerStub<ConfigureControllerOptions> {
    public:
        virtual ~ConfigureController();

        virtual void perform(const ConfigureControllerOptions &options) override;

    private:
        Version detectToolchainVersion() const;

        std::string detectTargetArchitecture() const;

        std::set<BuildType> generateBuildTypes(const Toolchain *, const std::string &buildTypeValue) const;

        std::unique_ptr<PackageRegistry> createPackageRegistry(PackageService *packageService, const boost::filesystem::path &packageRegistryPath) const;
    };
}

#endif
