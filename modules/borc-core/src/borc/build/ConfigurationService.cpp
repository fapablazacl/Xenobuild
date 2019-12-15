
#include "ConfigurationService.hpp"

#include <boost/filesystem.hpp>
#include <boost/hana.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/parsing/JSONDeserializer.hpp>
#include <borc/parsing/JSONSerializer.hpp>

#include "BuildCacheImpl.hpp"

BOOST_HANA_ADAPT_STRUCT(borc::Version, major, minor, revision);
BOOST_HANA_ADAPT_STRUCT(borc::BuildConfiguration, toolchainId, arch, version, buildTypes);
BOOST_HANA_ADAPT_STRUCT(borc::BuildType, type);

namespace borc {
    ConfigurationService::ConfigurationService(const boost::filesystem::path &outputPath) {
        this->outputPath = outputPath;

        this->loadConfigurations();
    }


    void ConfigurationService::addBuildConfiguration(const BuildConfiguration &newConfig) {
        auto it = configurationData.buildConfigurations.find(newConfig);

        if (it == configurationData.buildConfigurations.end()) {
            configurationData.buildConfigurations.insert(newConfig);
        } else {
            for (auto buildType : newConfig.buildTypes) {
                it->buildTypes.insert(buildType);
            }
        }
    }


    ConfigurationData ConfigurationService::getData() const {
        return configurationData;
    }


    void ConfigurationService::saveAllBuildConfigurations() {
        this->saveConfigurations();
    }
    

    void ConfigurationService::loadConfigurations() {
        const auto configFilePath = outputPath / "configuration.json";

        if (boost::filesystem::exists(configFilePath)) {
            auto fileService = FileServiceImpl{};

            const std::string configurationJsonContent = fileService.load(configFilePath.string());
            const nlohmann::json configurationJson = nlohmann::json::parse(configurationJsonContent);

            deserialize(configurationData.buildConfigurations, configurationJson);
        }
    }

    void ConfigurationService::saveConfigurations() {
        if (!boost::filesystem::exists(outputPath) && ! boost::filesystem::create_directory(outputPath) ) {
            throw std::runtime_error("Couldn't create the '" + outputPath.string() +  "' directory. Please check user/group/folder permissions");
        }

        const auto configFilePath = outputPath / "configuration.json";

        nlohmann::json configurationJson;
        serialize(configurationJson, configurationData.buildConfigurations);
        const std::string configurationJsonContent = configurationJson.dump(4);

        auto fileService = FileServiceImpl{};

        fileService.save(configFilePath.string(), configurationJsonContent);
    }

    std::unique_ptr<BuildCache> ConfigurationService::createBuildCache(const BuildConfiguration &config) {
        return std::make_unique<BuildCacheImpl>(outputPath / config.computeIdentifier());
    }
}
