
#include "BuildCacheImpl.hpp"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace borc {
    BuildCacheImpl::BuildCacheImpl(const boost::filesystem::path &outputPath) {
        this->outputPath = outputPath;

        this->loadCache();
    }

    BuildCacheImpl::~BuildCacheImpl() {
        this->saveCache();
    }

    std::string BuildCacheImpl::getCacheFileName() const {
        return (outputPath / "buildCache.txt").string();
    }

    void BuildCacheImpl::loadCache() {
        const std::string cacheFileName = this->getCacheFileName();

        std::fstream fs;
        fs.open(cacheFileName.c_str(), std::ios_base::in);

        if (! fs.is_open()) {
            return;
        }

        for (std::string line; std::getline(fs, line); ) {
            std::vector<std::string> parts;
            boost::algorithm::split(parts, line, boost::is_any_of(":"));

            pathTimeMap.insert({
                boost::filesystem::path{parts[0]},
                static_cast<std::time_t>(std::stoi(parts[1]))
            });
        }
    }

    void BuildCacheImpl::saveCache() {
        const std::string cacheFileName = this->getCacheFileName();

        std::fstream fs;
        fs.open(cacheFileName.c_str(), std::ios_base::out);

        if (! fs.is_open()) {
            return;
        }

        for (const auto &pair : pathTimeMap) {
            fs << pair.first << ":" << pair.second << std::endl;
        }
    }

    bool BuildCacheImpl::needsRebuild(const boost::filesystem::path &path) {
        bool modified = true;

        const std::time_t timestamp = boost::filesystem::last_write_time(path);

        if (auto it = pathTimeMap.find(path); it != pathTimeMap.end()) {
            modified = (timestamp != it->second);
        } 

        pathTimeMap.insert({path, timestamp});

        return modified;
    }
}
