
#include <borc/model/Artifact.hpp>

namespace borc {
    Artifact::Artifact() {}

    Artifact::~Artifact() {}

    std::vector<boost::filesystem::path> Artifact::computeSourceFiles() const {
        return {};
    }

    std::vector<boost::filesystem::path> Artifact::computeIncludePaths() const {
        return {};
    }
}
