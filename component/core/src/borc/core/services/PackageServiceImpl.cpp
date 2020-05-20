
#include "PackageServiceImpl.hpp"

#include <map>
#include <iostream>
#include <boost/range/algorithm/transform.hpp>
#include <boost/filesystem.hpp>

#include <borc/common/Constants.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/PackageRegistry.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Package.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/LanguageEntity.hpp>
#include <borc/entity/ComponentEntity.hpp>
#include <borc/entity/JsonModel.hpp>
#include <borc/entity/Decoder.hpp>
#include <borc/services/FileServiceImpl.hpp>

namespace bok {
    PackageServiceImpl::PackageServiceImpl(const FileService *fileService) {
        this->fileService = fileService;
    }


    std::unique_ptr<Package> PackageServiceImpl::createPackage(const boost::filesystem::path &packageBaseFolder, const PackageRegistry *packageRegistry) const {
        const PackageEntity packageEntity = this->loadPackageEntity(packageBaseFolder);

        if (packageEntity.modulePaths.size() > 0) {
            std::cout << "Creating package " << packageEntity.name << " ..." << std::endl;

            const std::vector<ComponentEntity> moduleEntities = this->loadModuleEntities(packageBaseFolder, packageEntity);
            std::unique_ptr<Package> package = this->createPackageImpl(packageEntity, moduleEntities, packageRegistry);

            return package;
        } else {
            /*
            // TODO: Put this validation in Configuration Time!
            if (packageEntity.required.size() > 0) {
                std::string msg;

                msg += "This package requires the following variables to be defined:\n";
                for (const auto &variable : packageEntity.required) {
                    msg += "    " + variable.name + ":" + variable.type + "\n";
                }

                throw std::runtime_error(msg);
            }
            */

            auto package = std::make_unique<Package>(packageEntity.name);

            std::vector<PackageVariable> variables;
            boost::range::transform(packageEntity.required, std::back_inserter(variables), [](const auto &required) {
                return PackageVariable{required.name};
            });

            package->setVariables(variables);

            for (const auto modulePackage : packageEntity.modules) {
                // TODO: Process "common" modules (serves as a set of base definitions)
                auto module = package->createModule<Module>();

                // module name
                module->setName(modulePackage.name);

                // include paths
                std::vector<boost::filesystem::path> includePaths;
                std::transform(
                    modulePackage.includePath.begin(), 
                    modulePackage.includePath.end(), 
                    std::back_inserter(includePaths), [](const std::string &includePath) {
                        return boost::filesystem::path(includePath);
                });

                module->setIncludePaths(includePaths);

                // library path
                std::vector<boost::filesystem::path> libraryPaths;
                std::transform(
                    modulePackage.libraryPath.begin(), 
                    modulePackage.libraryPath.end(), 
                    std::back_inserter(libraryPaths), [](const PackageEntity::LibraryPath &libraryPath) {
                        return boost::filesystem::path(libraryPath.path);
                });

                module->setLibraryPaths(libraryPaths);

                // import library
                module->setLibraries(modulePackage.library);
            }

            return package;
        }
    }


    std::unique_ptr<Package> PackageServiceImpl::createPackageImpl(const PackageEntity &packageEntity, const std::vector<ComponentEntity> &moduleEntities, const PackageRegistry *packageRegistry) const {
        // now we are ready to create the package and modules instances
        auto package = std::make_unique<Package>(packageEntity.name);

        // available module types for C/C++ projects
        const std::map<std::string, Module::Type> moduleTypeMap = {
            { "application/cli", Module::Type{"application", "cli"} },
            { "application/gui", Module::Type{"application", "gui"} },
            { "library/static", Module::Type{"library", "static"} },
            { "library/dynamic", Module::Type{"library", "dynamic"} }
        };

        for (int i=0; i<moduleEntities.size(); i++) {
            const ComponentEntity &componentEntity = moduleEntities[i];

            auto module = package->createModule<Module>();

            module->setName(componentEntity.name);

            if (auto moduleTypeIt = moduleTypeMap.find(componentEntity.type); moduleTypeIt != moduleTypeMap.end()) {
                module->setType(moduleTypeIt->second);
            } else {
                std::string msg;

                msg += "Invalid module type";
                msg += " '" + componentEntity.type + "' ";
                msg += "for the";
                msg += " '" + componentEntity.language + "' ";
                msg += "programming language specified in the";
                msg += " '" + componentEntity.name + "' ";
                msg += "module.";

                throw std::runtime_error(msg.c_str());
            }

            module->setPath(boost::filesystem::path{packageEntity.modulePaths[i]});

            std::vector<boost::filesystem::path> includePaths;
            std::vector<boost::filesystem::path> sourcePaths;

            for (const ComponentSourceEntity &moduleSourceEntity : componentEntity.sources) {
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
            const ComponentEntity &componentEntity = moduleEntities[i];
            Module *module = modules[i];

            for (const std::string dependency : componentEntity.dependencies) {
                // Solve the dependency with the modules inside the package
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

                // Solve the dependency as a external one
                if (! found && packageRegistry) {
                    auto dependentModule = packageRegistry->findModule(dependency);

                    if (dependentModule) {
                        auto dependencies = module->getDependencies();
                        dependencies.push_back(dependentModule);
                        module->setDependencies(dependencies);

                        found = true;
                    }
                }

                if (! found) {
                    std::string msg = "";

                    msg += "Required dependency ";
                    msg += dependency;
                    msg += " for module ";
                    msg += module->getName();
                    msg += " couldn't be found.";

                    throw std::runtime_error(msg);
                }
            }
        }

        return package;
    }


    PackageEntity PackageServiceImpl::loadPackageEntity(const boost::filesystem::path &packagePath) const {
        const auto packageFilePath = packagePath / BOK_PACKAGE_DEFINITION_FILENAME;

        if (! checkValidBorcFile(packageFilePath)) {
            throw std::runtime_error("There is no package build file on the folder '" + packageFilePath.string() + "'");
        }

        auto packageJsonContent = fileService->load(packageFilePath.string());
        auto packageJson = nlohmann::json::parse(packageJsonContent);

        return Decoder<JsonModel, PackageEntity>{packageJson}.decode();
    }


    std::vector<ComponentEntity> PackageServiceImpl::loadModuleEntities(const boost::filesystem::path &packagePath, const PackageEntity &packageEntity) const {
        std::vector<ComponentEntity> moduleEntities;

        for (const std::string &modulePartialPath : packageEntity.modulePaths) {
            const boost::filesystem::path moduleFilePath = packagePath / modulePartialPath / BOK_COMPONENT_DEFINITION_FILENAME;

            if (! checkValidBorcFile(moduleFilePath)) {
                throw std::runtime_error("There is no module build file on this folder '" + (packagePath / modulePartialPath).string() + "'");
            }

            auto moduleJsonContent = fileService->load(moduleFilePath.string());
            auto moduleJson = nlohmann::json::parse(moduleJsonContent);

            ComponentEntity componentEntity = Decoder<JsonModel, ComponentEntity>{moduleJson}.decode();

            moduleEntities.push_back(componentEntity);
        }

        return moduleEntities;
    }


    bool PackageServiceImpl::checkValidBorcFile(const boost::filesystem::path &filePath) const {
        return !boost::filesystem::is_directory(filePath) && boost::filesystem::exists(filePath);
    }
}
