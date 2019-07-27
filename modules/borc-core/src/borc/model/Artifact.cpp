
#include <borc/model/Artifact.hpp>

namespace borc {
    std::vector<boost::filesystem::path> Artifact::computeSourceFiles() const {
        return {};
    }

    std::vector<boost::filesystem::path> Artifact::computeIncludePaths() const {
        return {};
    }
}
