
#include <bok/core/toolchain/Compiler.hpp>

namespace bok {
    class Compiler_VC : public Compiler {
    public:
        CompileOutput generateCompileOutput(const CompileInput &input) const override;
    };
}
