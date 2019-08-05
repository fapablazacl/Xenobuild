
#include "ArtifactTypeChecker.hpp"

namespace borc {
    ArtifactTypeChecker::ArtifactTypeChecker(const std::initializer_list<Artifact::Type> &types) {
        for (Artifact::Type type : types) {
            this->types.insert(type);
        }
    }

    ArtifactTypeChecker::~ArtifactTypeChecker() {}

    bool ArtifactTypeChecker::check(const Artifact *artifact) const {
        for (Artifact::Type type : types) {
            if (type == artifact->getType()) {
                return true;
            }
        }

        return false;
    }
}
