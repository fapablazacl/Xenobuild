
#pragma once

#include <bok/core/toolchain/Toolchain.hpp>

namespace bok {
    class Toolchain_VC : public Toolchain {
    public:
        std::vector<Compiler*> enumerateCompilers() const override;

        std::vector<Linker*> enumerateLinkers() const override;

        Version detectVersion() const override;

        Compiler* getCompiler(const CompilerType type) const override;

        Linker* getLinker(const LinkerType type) const override;

    private:

    };
} 
