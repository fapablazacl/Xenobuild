
#include "PackageServiceImpl.hpp"

#include <map>
#include <boost/filesystem.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Package.hpp>
#include <borc/parsing/JSONDeserializer.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/LanguageEntity.hpp>
#include <borc/entity/ModuleEntity.hpp>


namespace borc {
    PackageServiceImpl::PackageServiceImpl(const FileService *fileService) {
        this->fileService = fileService;
    }


    std::unique_ptr<Package> PackageServiceImpl::createPackage(const boost::filesystem::path &packageBaseFolder) const {
        FileServiceImpl service;

        const PackageEntity packageEntity = this->loadPackageEntity(packageBaseFolder);
        const std::vector<ModuleEntity> moduleEntities = this->loadModuleEntities(packageBaseFolder, packageEntity);
        
        std::unique_ptr<Package> package = this->createPackageImpl(packageEntity, moduleEntities);

        return package;
    }


    std::unique_ptr<Package> PackageServiceImpl::createPackageImpl(const PackageEntity &packageEntity, const std::vector<ModuleEntity> &moduleEntities) const {
        // now we are ready to create the package and modules instances
        auto package = std::make_unique<Package>(packageEntity.name);

        // available module types for C/C++ projects
        const std::map<std::string, Module::Type> moduleTypeMap = {
            {"application/cli", Module::Type{"application", "cli"} },
            {"application/gui", Module::Type{"application", "gui"} },
            {"library/static", Module::Type{"library", "static"} },
            {"library/dynamic", Module::Type{"library", "dynamic"} }
        };

        for (int i=0; i<moduleEntities.size(); i++) {
            const ModuleEntity &moduleEntity = moduleEntities[i];

            Module *module = package->createModule();

            module->setName(moduleEntity.name);

            if (auto moduleTypeIt = moduleTypeMap.find(moduleEntity.type); moduleTypeIt != moduleTypeMap.end()) {
                module->setType(moduleTypeIt->second);
            } else {
                std::string msg;

                msg += "Invalid module type";
                msg += " '" + moduleEntity.type + "' ";
                msg += "for the";
                msg += " '" + moduleEntity.language + "' ";
                msg += "programming language specified in the";
                msg += " '" + moduleEntity.name + "' ";
                msg += "module.";

                throw std::runtime_error(msg.c_str());
            }

            module->setPath(boost::filesystem::path{packageEntity.modules[i]});

            std::vector<boost::filesystem::path> includePaths;
            std::vector<boost::filesystem::path> sourcePaths;

            for (const ModuleSourceEntity &moduleSourceEntity : moduleEntity.sources) {
                if (moduleSourceEntity.public_) {
                    includePaths.push_back(moduleSourceEntity.path);
                } else {
                    sourcePaths.push_back(moduleSourceEntity.path);
                }
            }

            module->setIncludePaths(includePaths);
            module->setSourcePaths(sourcePaths);
        }

        // solve module dependencies
        std::vector<Module*> modules = package->getModules();

        for (int i=0; i<moduleEntities.size(); i++) {
            const ModuleEntity &moduleEntity = moduleEntities[i];
            Module *module = modules[i];

            for (const std::string dependency :  moduleEntity.dependencies) {
                // TODO: Expand the dependency solving from the (future) build context object ...
                bool found = false;
                for (const Module *dependentModule : modules) {
                    if (dependency == dependentModule->getName()) {
                        auto dependencies = module->getDependencies();
                        dependencies.push_back(dependentModule);
                        module->setDependencies(dependencies);

                        found = true;
                        break;
                    }
                }

                if (! found) {
                    std::string msg;

                    msg += "Required dependency '" + dependency + "' ";
                    msg += "for module '" + module->getName() + "' couldn't be found.";

                    throw std::runtime_error(msg);
                }
            }
        }

        return package;
    }


    PackageEntity PackageServiceImpl::loadPackageEntity(const boost::filesystem::path &packagePath) const {
        const auto packageFilePath = packagePath / "package.borc.json";

        if (! checkValidBorcFile(packageFilePath)) {
            throw std::runtime_error("There is no package build file on the folder '" + packageFilePath.string() + "'");
        }

        auto packageJsonContent = fileService->load(packageFilePath.string());
        auto packageJson = nlohmann::json::parse(packageJsonContent);

        PackageEntity packageEntity;
        deserialize(packageEntity, packageJson);

        return packageEntity;
    }


    std::vector<ModuleEntity> PackageServiceImpl::loadModuleEntities(const boost::filesystem::path &packagePath, const PackageEntity &packageEntity) const {
        std::vector<ModuleEntity> moduleEntities;

        for (const std::string &modulePartialPath : packageEntity.modules) {
            const boost::filesystem::path moduleFilePath = packagePath / modulePartialPath / "module.borc.json";

            if (! checkValidBorcFile(moduleFilePath)) {
                throw std::runtime_error("There is no module build file on this folder '" + moduleFilePath.string() + "'");
            }

            auto moduleJsonContent = fileService->load(moduleFilePath.string());
            auto moduleJson = nlohmann::json::parse(moduleJsonContent);

            ModuleEntity moduleEntity;
            deserialize(moduleEntity, moduleJson);

            moduleEntities.push_back(moduleEntity);
        }

        return moduleEntities;
    }


    bool PackageServiceImpl::checkValidBorcFile(const boost::filesystem::path &filePath) const {
        return !boost::filesystem::is_directory(filePath) && boost::filesystem::exists(filePath);
    }
}
