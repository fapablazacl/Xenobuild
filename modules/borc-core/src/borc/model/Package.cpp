
#include <borc/model/Package.hpp>
#include <borc/model/Artifact.hpp>

namespace borc {
    Package::Package(const std::string &name) {
        this->name = name;
    }


    Package::~Package() {}


    std::vector<Artifact*> Package::getArtifacts() const {
        std::vector<Artifact*> result;

        for (auto &artifact : artifacts) {
            result.push_back(artifact.get());
        }

        return result;
    }


    Artifact* Package::createArtifact() {
        auto artifact = new Artifact(this);

        artifacts.emplace_back(artifact);

        return artifact;
    }
}
