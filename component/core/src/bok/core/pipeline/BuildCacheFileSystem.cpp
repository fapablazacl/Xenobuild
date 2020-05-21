
#include "BuildCacheFileSystem.hpp"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <bok/core/Constants.hpp>

namespace bok {
    BuildCacheFileSystem::BuildCacheFileSystem(const path &outputPath, const path &prefixPath) {
        this->outputPath = outputPath;
        this->prefixPath = prefixPath;
        this->loadCache();
    }


    BuildCacheFileSystem::~BuildCacheFileSystem() {}


    std::string BuildCacheFileSystem::getCacheFileName() const {
        return (outputPath / BOK_BUILD_CACHE_FILENAME).string();
    }


    void BuildCacheFileSystem::loadCache() {
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


    void BuildCacheFileSystem::saveCache() {
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


    bool BuildCacheFileSystem::needsRebuild(const boost::filesystem::path &path) const {
        bool modified = true;

        const auto timestamp = this->computeMark(path);
        const auto relativePath = boost::filesystem::relative(path, prefixPath);

        if (auto it = pathTimeMap.find(relativePath); it != pathTimeMap.end()) {
            return timestamp != it->second;
        } 

        return modified;
    }


    void BuildCacheFileSystem::markAsBuilt(const boost::filesystem::path &path) {
        const auto timestamp = this->computeMark(path);
        const auto relativePath = boost::filesystem::relative(path, prefixPath);

        pathTimeMap.insert({relativePath, timestamp});

        this->saveCache();
    }


    std::time_t BuildCacheFileSystem::computeMark(const boost::filesystem::path &path) const {
        return boost::filesystem::last_write_time(path);
    }
}