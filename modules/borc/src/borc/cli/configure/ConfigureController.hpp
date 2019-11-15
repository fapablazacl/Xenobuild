
#ifndef __BORC_CLI_CONFIGURECONTROLLER_HPP__
#define __BORC_CLI_CONFIGURECONTROLLER_HPP__

#include <set>
#include <borc/cli/ControllerStub.hpp>
#include "ConfigureControllerOptions.hpp"

namespace borc {

    struct Version;

    struct BuildType;
    class Toolchain;

    class ConfigureController : public ControllerStub<ConfigureControllerOptions> {
    public:
        virtual ~ConfigureController();

        virtual void perform(const ConfigureControllerOptions &options) override;

    private:
        Version detectToolchainVersion() const;

        std::string detectArchitecture() const;

        std::set<BuildType> generateBuildTypes(const Toolchain *, const std::string &buildTypeValue) const;
    };
}

#endif
