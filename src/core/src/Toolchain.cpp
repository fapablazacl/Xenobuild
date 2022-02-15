
#include <Xenobuild/core/Toolchain.h>

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


    CommandX createVCVars64Command(const boost::filesystem::path &prefixPath) {
        const auto vcvars = prefixPath / "VC\\Auxiliary\\Build\\vcvars64.bat";

        return { "call", { quote(vcvars.string()) } };
    }

    
    CommandBatch createToolchainCommandBatch(const CommandX command, const boost::filesystem::path &toolchainPrefix) {
        CommandBatch batch{};

        const CommandX vcvars = createVCVars64Command(toolchainPrefix);
        batch.commands.push_back(vcvars);

        batch.commands.push_back(command);

        return batch;
    }


    CommandBatch createToolchainCommandBatch(const CommandX command, const ToolchainType type, const boost::filesystem::path &toolchainPrefix) {
        CommandBatch batch{};

        if (type == ToolchainType::MicrosoftVC) {
            const CommandX vcvars = createVCVars64Command(toolchainPrefix);
            batch.commands.push_back(vcvars);
        }

        batch.commands.push_back(command);

        return batch;
    }


    Toolchain::Toolchain(const ToolchainType type) : type(type) {}

    
    CommandBatch Toolchain::createCommandBatch(const CommandX command) {
        if (installPath) {
            return createToolchainCommandBatch(command, installPath.get());
        }
        
        return CommandBatch{command};
    }
}
