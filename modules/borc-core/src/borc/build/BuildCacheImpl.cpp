
#include "BuildCacheImpl.hpp"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/hana.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/parsing/JSONDeserializer.hpp>
#include <borc/parsing/JSONSerializer.hpp>

BOOST_HANA_ADAPT_STRUCT(borc::Version, major, minor, revision);
BOOST_HANA_ADAPT_STRUCT(borc::BuildConfiguration, toolchainId, arch, version, buildTypes);
BOOST_HANA_ADAPT_STRUCT(borc::BuildType, type);

namespace borc {
    BuildCacheImpl::BuildCacheImpl(const boost::filesystem::path &outputPath) {
        this->outputPath = outputPath;
        this->loadCache();
        this->loadConfigurations();
    }

    BuildCacheImpl::~BuildCacheImpl() {}

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

            if (parts.size() >= 2) {
                pathTimeMap.insert({
                    boost::filesystem::path{parts[0]},
                    static_cast<std::time_t>(std::stoi(parts[1]))
                });
            }
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
            fs << pair.first.string() << ":" << pair.second << std::endl;
        }
    }

    bool BuildCacheImpl::needsRebuild(const boost::filesystem::path &path) const {
        bool modified = true;

        const auto timestamp = this->computeMark(path);

        if (auto it = pathTimeMap.find(path); it != pathTimeMap.end()) {
            return timestamp != it->second;
        } 

        return modified;
    }

    void BuildCacheImpl::markAsBuilt(const boost::filesystem::path &path) {
        const auto timestamp = this->computeMark(path);

        pathTimeMap.insert({path, timestamp});

        this->saveCache();
    }

    std::time_t BuildCacheImpl::computeMark(const boost::filesystem::path &path) const {
        return boost::filesystem::last_write_time(path);
    }

    void BuildCacheImpl::addBuildConfiguration(const BuildConfiguration &config) {
        buildCacheData.buildConfigurations.push_back(config);

        this->saveConfigurations();
    }

    void BuildCacheImpl::loadConfigurations() {
        const auto configFilePath = outputPath / "configuration.json";

        if (boost::filesystem::exists(configFilePath)) {
            auto fileService = FileServiceImpl{};

            const std::string configurationJsonContent = fileService.load(configFilePath.string());
            const nlohmann::json configurationJson = nlohmann::json::parse(configurationJsonContent);

            deserialize(buildCacheData.buildConfigurations, configurationJson);
        }
    }

    void BuildCacheImpl::saveConfigurations() {
        // loads the build configuration from the current path
        const auto configFilePath = outputPath / "configuration.json";

        nlohmann::json configurationJson;
        serialize(configurationJson, buildCacheData.buildConfigurations);
        const std::string configurationJsonContent = configurationJson.dump(4);

        auto fileService = FileServiceImpl{};

        fileService.save(configFilePath.string(), configurationJsonContent);
    }
}
