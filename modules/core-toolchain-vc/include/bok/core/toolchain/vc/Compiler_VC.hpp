
#pragma once

#include <bok/core/toolchain/Compiler.hpp>

#include <optional>

namespace bok {
    struct Compiler_VC_Path {
        boost::filesystem::path toolchainPath;
        boost::filesystem::path postfixPath;
    };

    class Compiler_VC : public Compiler {
    public:
        explicit Compiler_VC(std::optional<Compiler_VC_Path> path);
        
        CompileOutput generateCompileOutput(const CompileInput &input) const override;

    private:
        CompileInput intercept(CompileInput input) const;

    private:
        std::optional<Compiler_VC_Path> path;
    };
}
