
#pragma once

#include <bok/core/toolchain/Compiler.hpp>

#include <optional>

namespace bok {
    class Compiler_VC : public Compiler {
    public:
        explicit Compiler_VC(std::optional<std::string> path);
        
        CompileOutput generateCompileOutput(const CompileInput &input) const override;

    private:
        std::optional<std::string> path;
    };
}
