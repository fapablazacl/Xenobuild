
#pragma once

#include <bok/core/toolchain/Toolchain.hpp>

#include <memory>
#include <bok/core/toolchain/vc/Compiler_VC.hpp>
#include <bok/core/toolchain/vc/Linker_VC.hpp>

namespace bok {
    class Toolchain_VC : public Toolchain {
    public:
        Toolchain_VC();

        ~Toolchain_VC();

        std::vector<Compiler*> enumerateCompilers() const override;

        std::vector<Linker*> enumerateLinkers() const override;

        Version detectVersion() const override;

        Compiler* getCompiler(const CompilerType type) const override;

        Linker* getLinker(const LinkerType type) const override;

    private:
        std::vector<std::unique_ptr<Compiler_VC>> compilers;
        std::vector<std::unique_ptr<Linker_VC>> linkers;
    };
}
