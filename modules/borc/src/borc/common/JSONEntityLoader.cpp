
#include "JSONEntityLoader.hpp"

#include <stdexcept>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <borc/parsing/JSONDeserializer.hpp>
#include <borc/services/FileService.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/ModuleEntity.hpp>

namespace borc {
    JSONEntityLoader::JSONEntityLoader(const boost::filesystem::path &basePath_, FileService &fileService_) 
        : basePath(basePath_), fileService(fileService_) {}

    JSONEntityLoader::~JSONEntityLoader() {}

    PackageEntity JSONEntityLoader::loadPackageEntity() const {
        const auto packageFilePath = basePath / "borc-package.json";

        if (! checkValidBorcFile(packageFilePath)) {
            throw std::runtime_error("There is no package build file on the folder '" + packageFilePath.string() + "'");
        }

        auto packageJsonContent = fileService.load(packageFilePath.string());
        auto packageJson = nlohmann::json::parse(packageJsonContent);

        PackageEntity packageEntity;
        deserialize(packageEntity, packageJson);

        return packageEntity;
    }


    std::vector<ModuleEntity> JSONEntityLoader::loadModuleEntities(const PackageEntity &packageEntity) const {
        std::vector<ModuleEntity> moduleEntities;

        for (const std::string &modulePartialPath : packageEntity.modules) {
            const boost::filesystem::path moduleFilePath = basePath / modulePartialPath / "borc-artifact.json";

            if (! checkValidBorcFile(moduleFilePath)) {
                throw std::runtime_error("There is no artifact build file on this folder '" + moduleFilePath.string() + "'");
            }

            auto moduleJsonContent = fileService.load(moduleFilePath.string());
            auto moduleJson = nlohmann::json::parse(moduleJsonContent);

            ModuleEntity moduleEntity;
            deserialize(moduleEntity, moduleJson);

            moduleEntities.push_back(moduleEntity);
        }

        return moduleEntities;
    }

    bool JSONEntityLoader::checkValidBorcFile(const boost::filesystem::path &filePath) const {
        return !boost::filesystem::is_directory(filePath) && boost::filesystem::exists(filePath);
    }
}
