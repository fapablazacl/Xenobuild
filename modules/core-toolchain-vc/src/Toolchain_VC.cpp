
#include <bok/core/toolchain/vc/Toolchain_VC.hpp>

#include <cassert>
#include <boost/filesystem.hpp>
#include <bok/core/Version.hpp>


namespace bok {
    class PrebuiltPackage_StdLib : public PrebuiltPackage {
    public:
        PrebuiltPackage_StdLib(const boost::filesystem::path &basePath, const std::string &targetArch)
            : basePath(basePath), targetArch(targetArch) {

            assert(boost::filesystem::exists(basePath));
        }

        std::string getName() const override {
            return "stdlib";
        }

        PrebuiltPackageBuildData getBuildData() const override {
            PrebuiltPackageBuildData result;

            result.includePaths = { basePath / "include" };
            result.libraryPaths = { basePath / "lib" / targetArch };

            return result;
        }

    private:
        boost::filesystem::path basePath;
        std::string targetArch;
    };


    class PrebuiltPackage_WinSDK : public PrebuiltPackage {
    public:
        PrebuiltPackage_WinSDK(const boost::filesystem::path &basePath, const std::string &version, const std::string &targetArch)
            : basePath(basePath), version(version), targetArch(targetArch) {

            assert(boost::filesystem::exists(basePath));
        }

        std::string getName() const override {
            return "winsdk";
        }

        PrebuiltPackageBuildData getBuildData() const override {
            PrebuiltPackageBuildData result;

            result.includePaths = {
                basePath / "Include" / version / "ucrt",
                basePath / "Include" / version / "um",
                basePath / "Include" / version / "shared"
            };

            result.libraryPaths = {
                basePath / "Lib" / version / "um" / targetArch,
                basePath / "Lib" / version / "ucrt" / targetArch
            };

            result.libraries = {"AdvAPI32"};

            return result;
        }

    private:
        boost::filesystem::path basePath;
        std::string version;
        std::string targetArch;
    };


    Toolchain_VC::Toolchain_VC(std::optional<boost::filesystem::path> toolchainPath) : toolchainPath(toolchainPath) {
        // TODO: This depends on the host/target architecture
        const std::string postfix = computePostfix();

        if (toolchainPath) {
            compilers.emplace_back(new Compiler_VC{ Compiler_VC_Path{*toolchainPath, postfix} });

            linkers.emplace_back(new Linker_VC{*toolchainPath / postfix});
        } else {
            compilers.emplace_back(new Compiler_VC{ {} });
            linkers.emplace_back(new Linker_VC{ {} });
        }
    }

    Toolchain_VC::~Toolchain_VC() {}

    std::vector<Compiler*> Toolchain_VC::enumerateCompilers() const {
        return { compilers[0].get() };
    }

    std::vector<Linker*> Toolchain_VC::enumerateLinkers() const {
        return { linkers[0].get() };
    }

    Version Toolchain_VC::detectVersion() const {
        // TODO: Compute toolchain version
        return {1, 0, 0};
    }

    Compiler* Toolchain_VC::getCompiler(const CompilerType type) const {
        if (type == CompilerType::Source) {
            return compilers[0].get();
        }

        return nullptr;
    }

    Linker* Toolchain_VC::getLinker(const LinkerType type) const {
        if (type == LinkerType::Binary) {
            return linkers[0].get();
        }

        return nullptr;
    }

    std::string Toolchain_VC::computePostfix() const {
        return "bin/Hostx64/x64";
    }

    std::optional<PrebuiltPackageBuildData> Toolchain_VC::getPackageBuildData(const std::string& packageName) const {
        if (packageName == "winsdk") {
            const auto winsdk = PrebuiltPackage_WinSDK {
                "C:\\Program Files (x86)\\Windows Kits\\10",
                "10.0.18362.0",
                "x64"
            };

            assert(winsdk.getName() == "winsdk");

            if (toolchainPath) {
                const auto stdlib = PrebuiltPackage_StdLib {*toolchainPath, "x64" };
                assert(stdlib.getName() == "stdlib");

                return stdlib.getBuildData().merge(winsdk.getBuildData());
            }

            return winsdk.getBuildData();
        }

        return {};
    }
}
