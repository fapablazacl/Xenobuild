
#include <bok/toolchain/SourceChecker.hpp>
#include <algorithm>

namespace bok {
    SourceChecker::SourceChecker(const std::initializer_list<std::string> &wildcards) {
        this->wildcards = wildcards;
    }

    bool SourceChecker::check(const boost::filesystem::path &filePath) const {
        const std::string ext = "*" + filePath.extension().string();

        auto it = std::find(wildcards.begin(), wildcards.end(), ext);

        return it != wildcards.end();
    }
}
