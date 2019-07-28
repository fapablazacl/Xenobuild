
#ifndef __BORC_MODEL_ARTIFACT_HPP__
#define __BORC_MODEL_ARTIFACT_HPP__

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace borc {
    struct Version {
        int major;
        int minor;
        int revision;
    };

    class Package;
    class Artifact {
    public:
        enum class Type {
            ApplicationCli,
            ApplicationGui,
            LibraryStatic,
            LibraryDynamic
        };

    public:
        explicit Artifact(Package *package);

        ~Artifact();

        std::string getName() const {
            return name;
        }

        Type getType() const {
            return type;
        }

        boost::filesystem::path getPath() const {
            return path;
        }

        const Package* getPackage() const {
            return package;
        }

        Version getVersion() const {
            return version;
        }

        std::vector<const Artifact*> getDependencies() const {
            return dependencies;
        }

        std::vector<boost::filesystem::path> getSourceFiles() const {
            return sourcePaths;
        }

        std::vector<boost::filesystem::path> getIncludePaths() const {
            return includePaths;
        }

        void setName(const std::string &name);

        void setPath(const boost::filesystem::path &path);

        void setType(const Type type);

        void setVersion(const Version &version);

        void setSourcePaths(const std::vector<boost::filesystem::path> &sourcePaths);

        void setIncludePaths(const std::vector<boost::filesystem::path> &includePaths);

    private:
        Package *package = nullptr;
        std::string name;
        boost::filesystem::path path;
        Type type;
        Version version;
        std::vector<const Artifact*> dependencies;

        std::vector<boost::filesystem::path> sourcePaths;
        std::vector<boost::filesystem::path> includePaths;
    };
}

#endif
