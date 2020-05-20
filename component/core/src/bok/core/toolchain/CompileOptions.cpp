
#include <bok/toolchain/CompileOptions.hpp>

namespace bok {
    void CompileOptions::mergeWith(const CompileOptions &other) {
        includePaths.insert(includePaths.end(), other.includePaths.begin(), other.includePaths.end());
    }
}
