
#include <bok/core/toolchain/ToolchainMock.hpp>

#include <algorithm>
#include <bok/core/Version.hpp>
#include <bok/core/toolchain/CompilerMock.hpp>
#include <bok/core/toolchain/LinkerMock.hpp>

namespace bok {
    ToolchainMock::ToolchainMock() {
        compilers.emplace_back(new CompilerMock{});
        linkers.emplace_back(new LinkerMock{});
    }

    ToolchainMock::~ToolchainMock() {}

    std::vector<Compiler*> ToolchainMock::enumerateCompilers() const {
        std::vector<Compiler*> result;

        result.push_back(compilers[0].get());

        return result;
    }

    std::vector<Linker*> ToolchainMock::enumerateLinkers() const {
        std::vector<Linker*> result;

        result.push_back(linkers[0].get());

        return result;
    }

    Version ToolchainMock::detectVersion() const {
        return { 1, 0, 0 };
    }
}
