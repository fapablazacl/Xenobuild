
#pragma once

#include <bok/core/toolchain/Toolchain.hpp>

#include <vector>
#include <memory>

namespace bok {
    class Compiler;
    class Linker;
    class ToolchainMock : public Toolchain {
    public:
        ToolchainMock();

        ~ToolchainMock() override;

        std::vector<Compiler*> enumerateCompilers() const override;

        std::vector<Linker*> enumerateLinkers() const override;

        Version detectVersion() const override;

    private:
        std::vector<std::unique_ptr<Compiler>> compilers;
        std::vector<std::unique_ptr<Linker>> linkers;
    };
}
