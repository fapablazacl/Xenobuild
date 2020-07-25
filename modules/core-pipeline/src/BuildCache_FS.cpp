
#include <bok/core/pipeline/BuildCache_FS.hpp>

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <bok/core/Constants.hpp>

namespace bok {
    BuildCache_FS::BuildCache_FS(const path &outputPath, const path &prefixPath) {
        this->outputPath = outputPath;
        this->prefixPath = prefixPath;
        this->loadCache();
    }


    BuildCache_FS::~BuildCache_FS() {}


    std::string BuildCache_FS::getCacheFileName() const {
        return (outputPath / BOK_BUILD_CACHE_FILENAME).string();
    }


    void BuildCache_FS::loadCache() {
        const std::string cacheFileName = this->getCacheFileName();

        std::fstream fs;
        fs.open(cacheFileName.c_str(), std::ios_base::in);

        if (! fs.is_open()) {
            return;
        }

        for (std::string line; std::getline(fs, line); ) {
            std::vector<std::string> parts;
            boost::algorithm::split(parts, line, boost::is_any_of(":"));

            if (parts.size() >= 2) {
                pathTimeMap.insert({
                    boost::filesystem::path{parts[0]},
                    static_cast<std::time_t>(std::stoi(parts[1]))
                });
            }
        }
    }


    void BuildCache_FS::saveCache() {
        const std::string cacheFileName = this->getCacheFileName();

        std::fstream fs;
        fs.open(cacheFileName.c_str(), std::ios_base::out);

        if (! fs.is_open()) {
            return;
        }

        for (const auto &pair : pathTimeMap) {
            fs << pair.first.string() << ":" << pair.second << std::endl;
        }
    }


    bool BuildCache_FS::needsRebuild(const boost::filesystem::path &path) const {
        bool modified = true;

        const auto timestamp = this->computeMark(path);
        const auto relativePath = boost::filesystem::relative(path, prefixPath);

        if (auto it = pathTimeMap.find(relativePath); it != pathTimeMap.end()) {
            return timestamp != it->second;
        } 

        return modified;
    }


    void BuildCache_FS::markAsBuilt(const boost::filesystem::path &path) {
        const auto timestamp = this->computeMark(path);
        const auto relativePath = boost::filesystem::relative(path, prefixPath);

        pathTimeMap.insert({relativePath, timestamp});

        this->saveCache();
    }


    std::time_t BuildCache_FS::computeMark(const boost::filesystem::path &path) const {
        return boost::filesystem::last_write_time(path);
    }
}
