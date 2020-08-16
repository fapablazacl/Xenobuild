
#pragma once

#include <bok/core/toolchain/Toolchain.hpp>

#include <memory>
#include <optional>
#include <bok/core/toolchain/vc/Compiler_VC.hpp>
#include <bok/core/toolchain/vc/Linker_VC.hpp>

namespace bok {
    class Toolchain_VC : public Toolchain {
    public:
        explicit Toolchain_VC(std::optional<boost::filesystem::path> toolchainPath);

        ~Toolchain_VC();

        std::vector<Compiler*> enumerateCompilers() const override;

        std::vector<Linker*> enumerateLinkers() const override;

        Version detectVersion() const override;

        Compiler* getCompiler(const CompilerType type) const override;

        Linker* getLinker(const LinkerType type) const override;

        std::optional<PrebuiltPackageBuildData> getPackageBuildData(const std::string &packageName) const override;

    private:
        std::string computePostfix() const;

    private:
        std::optional<boost::filesystem::path> toolchainPath;
        std::vector<std::unique_ptr<Compiler_VC>> compilers;
        std::vector<std::unique_ptr<Linker_VC>> linkers;
    };
}
