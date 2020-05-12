
#include "BuildCacheTxt.hpp"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace borc {
    BuildCacheTxt::BuildCacheTxt(const boost::filesystem::path &outputPath) {
        this->outputPath = outputPath;
        this->loadCache();
        
    }

    BuildCacheTxt::~BuildCacheTxt() {}

    std::string BuildCacheTxt::getCacheFileName() const {
        return (outputPath / "buildCache.txt").string();
    }

    void BuildCacheTxt::loadCache() {
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

    void BuildCacheTxt::saveCache() {
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

    bool BuildCacheTxt::needsRebuild(const boost::filesystem::path &path) const {
        bool modified = true;

        const auto timestamp = this->computeMark(path);

        if (auto it = pathTimeMap.find(path); it != pathTimeMap.end()) {
            return timestamp != it->second;
        } 

        return modified;
    }

    void BuildCacheTxt::markAsBuilt(const boost::filesystem::path &path) {
        const auto timestamp = this->computeMark(path);

        pathTimeMap.insert({path, timestamp});

        this->saveCache();
    }

    std::time_t BuildCacheTxt::computeMark(const boost::filesystem::path &path) const {
        return boost::filesystem::last_write_time(path);
    }

}