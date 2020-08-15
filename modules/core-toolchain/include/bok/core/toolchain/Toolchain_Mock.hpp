
#pragma once

#include <bok/core/toolchain/Toolchain.hpp>

#include <vector>
#include <memory>

namespace bok {
    class Compiler;
    class Linker;
    class Toolchain_Mock : public Toolchain {
    public:
        Toolchain_Mock();

        ~Toolchain_Mock() override;

        std::vector<Compiler*> enumerateCompilers() const override;

        std::vector<Linker*> enumerateLinkers() const override;

        Version detectVersion() const override;

        Compiler* getCompiler(const CompilerType type) const override;

        Linker* getLinker(const LinkerType type) const override;

    private:
        std::vector<std::unique_ptr<Compiler>> compilers;
        std::vector<std::unique_ptr<Linker>> linkers;
    };
}
