
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

        void setName(const std::string &name);

        void setPath(const boost::filesystem::path &path);

        void setType(const Type type);

        void setVersion(const Version &version);

        std::vector<boost::filesystem::path> computeSourceFiles() const;

        std::vector<boost::filesystem::path> computeIncludePaths() const;

    private:
        Package *package = nullptr;
        std::string name;
        boost::filesystem::path path;
        Type type;
        Version version;
        std::vector<const Artifact*> dependencies;
    };
}

#endif
