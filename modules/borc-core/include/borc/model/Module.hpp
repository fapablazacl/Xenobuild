
#ifndef __BORC_MODEL_ARTIFACT_HPP__
#define __BORC_MODEL_ARTIFACT_HPP__

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

#include <borc/model/Version.hpp>
#include <borc/model/Language.hpp>

namespace borc {
    class Package;
    class Source;
    class Module {
    public:
        struct Type {
            std::string tag;
            std::string subTag;

            bool operator== (const Type &other) const {
                return tag==other.tag && subTag==other.subTag;
            }

            bool operator!= (const Type &other) const {
                return ! (*this == other);
            }

            bool operator< (const Type &other) const {
                if (tag < other.tag) {
                    return true;
                }

                return subTag < other.subTag;
            }
        };

    public:
        explicit Module(Package *package);

        ~Module();

        std::string getName() const {
            return name;
        }

        Type getType() const {
            return type;
        }

        Language getLanguage() const {
            return language;
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

        std::vector<const Module*> getDependencies() const {
            return dependencies;
        }

        std::vector<boost::filesystem::path> getSourcePaths() const {
            return sourcePaths;
        }

        std::vector<boost::filesystem::path> getIncludePaths() const {
            return includePaths;
        }

        void setName(const std::string &name);

        void setLanguage(const Language &lang);

        void setPath(const boost::filesystem::path &path);

        void setType(const Type type);

        void setVersion(const Version &version);

        void setSourcePaths(const std::vector<boost::filesystem::path> &sourcePaths);

        void setIncludePaths(const std::vector<boost::filesystem::path> &includePaths);

        /**
         * @brief Reconstruct all the internal object Source files based on the supplied base folder
         */
        void rescanSources(const boost::filesystem::path &baseFolder);

        std::vector<Source*> getSources() const;

        void setDependencies(const std::vector<const Module*> &dependentModules);

    private:
        Package *package = nullptr;
        std::string name;
        boost::filesystem::path path;
        Type type;
        Version version;
        Language language;
        std::vector<const Module*> dependencies;

        std::vector<boost::filesystem::path> sourcePaths;
        std::vector<boost::filesystem::path> includePaths;

        std::vector<std::unique_ptr<Source>> sources;
    };
}

#endif
