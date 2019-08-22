
#include <borc/model/Context.hpp>
#include <boost/filesystem.hpp>

namespace borc {
    Context::Context(const boost::filesystem::path &outputPath, Package *package, std::vector<std::unique_ptr<Package>> packages) {
        this->outputPath = outputPath;
        this->package = package;
        this->packages = std::move(packages);

        // load 
    }

    Context::~Context() {
        // save
    }

    std::vector<Package*> Context::getPackages() const {
        std::vector<Package*> packages;

        for (const auto &package : this->packages) {
            packages.push_back(package.get());
        }

        return packages;
    }

    Package* Context::getMainPackage() const {
        return this->package;
    }

    bool Context::isModified(const boost::filesystem::path &path) {
        bool modified = true;

        const std::time_t timestamp = boost::filesystem::last_write_time(path);

        if (auto it = pathTimeMap.find(path); it != pathTimeMap.end()) {
            modified = (timestamp != it->second);
        } 

        pathTimeMap.insert({path, timestamp});

        return modified;
    }
}
