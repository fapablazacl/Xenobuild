
#include <borc/model/Artifact.hpp>
#include <borc/model/Package.hpp>

#include <boost/filesystem.hpp>

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

    std::vector<boost::filesystem::path> Artifact::getSourceFiles(const boost::filesystem::path &baseFolder) const {
        std::vector<boost::filesystem::path> sourceFiles;

        for (const boost::filesystem::path &sourcePath : sourcePaths) {
            const boost::filesystem::path solvedSourcePath = baseFolder / path / sourcePath;

            if (! boost::filesystem::exists(solvedSourcePath)) {
                continue;
            }

            if (boost::filesystem::is_directory(solvedSourcePath)) {
                boost::filesystem::recursive_directory_iterator it{solvedSourcePath}, end;

                while (it != end) {
                    if (! boost::filesystem::is_directory(it->path())) {
                        sourceFiles.push_back(solvedSourcePath);
                    }

                    ++it;
                }

            } else {
                sourceFiles.push_back(solvedSourcePath);
            }
        }

        return sourceFiles;
    }
}
