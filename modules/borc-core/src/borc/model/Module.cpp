
#include <borc/model/Module.hpp>

#include <boost/filesystem.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/Source.hpp>


namespace borc {
    Module::Module(Package *package) {
        this->package = package;
    }

    Module::~Module() {}

    void Module::setName(const std::string &name) {
        this->name = name;
    }

    void Module::setPath(const boost::filesystem::path &path) {
        this->path = path;
    }

    void Module::setType(const Module::Type type) {
        this->type = type;
    }

    void Module::setVersion(const Version &version) {
        this->version = version;
    }

    void Module::setSourcePaths(const std::vector<boost::filesystem::path> &sourcePaths) {
        this->sourcePaths = sourcePaths;
    }

    void Module::setIncludePaths(const std::vector<boost::filesystem::path> &includePaths) {
        this->includePaths = includePaths;
    }

    void Module::setDependencies(const std::vector<const Module*> &dependentModules) {
        dependencies = dependentModules; 
    }

    void Module::setLanguage(const Language &lang) {
        language = lang;
    }

    void Module::rescanSources(const boost::filesystem::path &baseFolder) {
        sources.clear();

        for (const boost::filesystem::path &sourcePath : sourcePaths) {
            const boost::filesystem::path solvedSourcePath = baseFolder / path / sourcePath;

            if (! boost::filesystem::exists(solvedSourcePath)) {
                continue;
            }

            if (boost::filesystem::is_directory(solvedSourcePath)) {
                boost::filesystem::recursive_directory_iterator it{solvedSourcePath}, end;

                while (it != end) {
                    if (! boost::filesystem::is_directory(it->path())) {
                        const auto filePath = it->path();
                        const auto relativeFilePath = boost::filesystem::relative(filePath, baseFolder);

                        auto source = new Source{this, filePath, relativeFilePath};

                        sources.emplace_back(source);
                    }

                    ++it;
                }
            } else {
                auto source = new Source(this, solvedSourcePath, solvedSourcePath);
                sources.emplace_back(source);
            }
        }
    }

    std::vector<Source*> Module::getSources() const {
        std::vector<Source*> result;

        for (auto &source : sources) {
            result.push_back(source.get());
        }

        return result;
    }

    void Module::setLibraryPaths(const std::vector<boost::filesystem::path> &libraryPaths) {
        this->libraryPaths = libraryPaths;
    }

    void Module::setLibraries(const std::vector<std::string> &libraries) {
        this->libraries = libraries;
    }
}
