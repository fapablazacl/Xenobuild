
#include <borc/model/Artifact.hpp>

namespace borc {
    Artifact::Artifact(Package *package) {
        this->package = package;
    }

    Artifact::~Artifact() {}

    void Artifact::setName(const std::string &name) {
        this->name = name;
    }

    void Artifact::setPath(const boost::filesystem::path &path) {
        this->path = path;
    }

    void Artifact::setType(const Artifact::Type type) {
        this->type = type;
    }

    void Artifact::setVersion(const Version &version) {
        this->version = version;
    }

    void Artifact::setSourcePaths(const std::vector<boost::filesystem::path> &sourcePaths) {
        this->sourcePaths = sourcePaths;
    }

    void Artifact::setIncludePaths(const std::vector<boost::filesystem::path> &includePaths) {
        this->includePaths = includePaths;
    }
}
