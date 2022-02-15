
#include <Xenobuild/core/Toolchain.h>

#include <cassert>
#include <Xenobuild/core/Triplet.h>
#include <Xenobuild/core/Util.h>
#include <Xenobuild/core/Command.h>


namespace Xenobuild {
    std::vector<std::string> enumerateVCInstallations() {
        const std::vector<std::string> vars = {
            "VS2019INSTALLDIR"
        };

        std::vector<std::string> installations;

        for (const std::string& var : vars) {
            const boost::optional<std::string> value = getenv(var);

            if (! value.has_value()) {
                continue;
            }

            installations.push_back(value.get());
        }

        return installations;
    }


    CommandX createVCVarsCommand(const boost::filesystem::path &prefixPath) {
        const auto vcvars = prefixPath / "VC\\Auxiliary\\Build\\vcvarsall.bat";
        
        return { "call", { quote(vcvars.string()) } };
    }


    std::vector<std::string> ToolchainInstallPathEnumerator::enumerate(const ToolchainType type) const {
        if (type == ToolchainType::MicrosoftVC) {
            return enumerateVCInstallations();
        }

        return {};
    }


    Toolchain::Toolchain(const Triplet &triplet, const std::string &installPath) 
        : triplet(triplet), installPath(installPath) {
    
        assert(triplet.platform.os == OS::Host);
        assert(triplet.platform.arch == Arch::Native);
    }


    boost::optional<CommandX> Toolchain::createEnvCommand() const {
        if (triplet.toolchainType == ToolchainType::MicrosoftVC) {
            return createVCVarsCommand(installPath);
        }

        return {};
    }
}
