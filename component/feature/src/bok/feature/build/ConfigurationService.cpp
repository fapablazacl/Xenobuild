
#include "ConfigurationService.hpp"

#include <boost/filesystem.hpp>
#include <boost/hana.hpp>
#include <bok/core/common/FileServiceImpl.hpp>
#include <bok/core/io/JsonModel.hpp>
#include <bok/core/io/Decoder.hpp>
#include <bok/core/io/Encoder.hpp>
#include <bok/core/pipeline/BuildCacheTxt.hpp>

BOOST_HANA_ADAPT_STRUCT(bok::Version, major, minor, revision);
BOOST_HANA_ADAPT_STRUCT(bok::BuildConfiguration, toolchainId, arch, version, buildTypes, variables, toolchainPath);
BOOST_HANA_ADAPT_STRUCT(bok::BuildType, type);

namespace bok {
    ConfigurationService::ConfigurationService(const boost::filesystem::path &outputPath, const path &prefixPath) {
        this->outputPath = outputPath;
        this->prefixPath = prefixPath;

        this->loadConfigurations();
    }


    void ConfigurationService::addBuildConfiguration(const BuildConfiguration &newConfig) {
        auto it = configurationData.buildConfigurations.find(newConfig);

        if (it == configurationData.buildConfigurations.end()) {
            configurationData.buildConfigurations.insert(newConfig);
        } else {
            for (auto buildType : newConfig.buildTypes) {
                it->buildTypes.insert(buildType);

                // merge the variables
                for (const auto &pair : newConfig.variables) {
                    it->variables.insert(pair);
                }

                it->toolchainPath = newConfig.toolchainPath;
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

            configurationData.buildConfigurations = Decoder<JsonModel, std::set<BuildConfiguration>>{configurationJson}.decode();
        }
    }

    void ConfigurationService::saveConfigurations() {
        if (!boost::filesystem::exists(outputPath) && !boost::filesystem::create_directory(outputPath) ) {
            throw std::runtime_error("Couldn't create the '" + outputPath.string() +  "' directory. Please check user/group/folder permissions.");
        }

        const auto configFilePath = outputPath / "configuration.json";

        JsonModel configurationJson = Encoder<JsonModel, std::set<BuildConfiguration>>{configurationData.buildConfigurations}.encode();

        const std::string configurationJsonContent = configurationJson.dump(4);

        auto fileService = FileServiceImpl{};

        fileService.save(configFilePath.string(), configurationJsonContent);
    }


    std::unique_ptr<BuildCache> ConfigurationService::createBuildCache(const BuildConfiguration &config) {
        return std::make_unique<BuildCacheTxt>(outputPath / config.computeIdentifier(), prefixPath);
    }
}
