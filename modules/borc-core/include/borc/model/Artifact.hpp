
#ifndef __BORC_MODEL_ARTIFACT_HPP__
#define __BORC_MODEL_ARTIFACT_HPP__

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace borc {
    struct Language {
        std::string name;
        std::string dialect;
    };

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
        Artifact(const Package *package, const std::string &name, const boost::filesystem::path &path, const Type type);

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

        std::vector<boost::filesystem::path> computeSourceFiles() const;

        std::vector<boost::filesystem::path> computeIncludePaths() const;

        std::vector<const Artifact*> getDependencies() const {
            return dependencies;
        }

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
