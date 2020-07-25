
#include <bok/core/package/PackageFactory_FS.hpp>

#include <map>
#include <iostream>
#include <boost/range/algorithm/transform.hpp>
#include <boost/filesystem.hpp>

#include <bok/core/Constants.hpp>
#include <bok/core/FileServiceImpl.hpp>
#include <bok/core/package/Package.hpp>
#include <bok/core/package/PackageRegistry.hpp>
#include <bok/core/package/Module.hpp>
#include <bok/core/package/Package.hpp>
#include <bok/core/io/PackageEntity.hpp>
#include <bok/core/io/LanguageEntity.hpp>
#include <bok/core/io/ComponentEntity.hpp>
#include <bok/core/io/JsonModel.hpp>
#include <bok/core/io/Decoder.hpp>

namespace bok {
    PackageFactory_FS::PackageFactory_FS(const FileService *fileService) {
        this->fileService = fileService;
    }


    std::unique_ptr<Package> PackageFactory_FS::createPackage(const boost::filesystem::path &packageBaseFolder, const PackageRegistry *packageRegistry) const {
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
                auto component = package->createModule<Module>();

                // component name
                component->setName(modulePackage.name);

                // include paths
                std::vector<boost::filesystem::path> includePaths;
                std::transform(
                    modulePackage.includePath.begin(), 
                    modulePackage.includePath.end(), 
                    std::back_inserter(includePaths), [](const std::string &includePath) {
                        return boost::filesystem::path(includePath);
                });

                component->setIncludePaths(includePaths);

                // library path
                std::vector<boost::filesystem::path> libraryPaths;
                std::transform(
                    modulePackage.libraryPath.begin(), 
                    modulePackage.libraryPath.end(), 
                    std::back_inserter(libraryPaths), [](const PackageEntity::LibraryPath &libraryPath) {
                        return boost::filesystem::path(libraryPath.path);
                });

                component->setLibraryPaths(libraryPaths);

                // import library
                component->setLibraries(modulePackage.library);
            }

            return package;
        }
    }


    std::unique_ptr<Package> PackageFactory_FS::createPackageImpl(const PackageEntity &packageEntity, const std::vector<ComponentEntity> &moduleEntities, const PackageRegistry *packageRegistry) const {
        // now we are ready to create the package and modules instances
        auto package = std::make_unique<Package>(packageEntity.name);

        // available component types for C/C++ projects
        const std::map<std::string, Module::Type> moduleTypeMap = {
            { "application/cli", Module::Type{"application", "cli"} },
            { "application/gui", Module::Type{"application", "gui"} },
            { "library/static", Module::Type{"library", "static"} },
            { "library/dynamic", Module::Type{"library", "dynamic"} }
        };

        for (int i=0; i<moduleEntities.size(); i++) {
            const ComponentEntity &componentEntity = moduleEntities[i];

            auto component = package->createModule<Module>();

            component->setName(componentEntity.name);

            if (auto moduleTypeIt = moduleTypeMap.find(componentEntity.type); moduleTypeIt != moduleTypeMap.end()) {
                component->setType(moduleTypeIt->second);
            } else {
                std::string msg;

                msg += "Invalid component type";
                msg += " '" + componentEntity.type + "' ";
                msg += "for the";
                msg += " '" + componentEntity.language + "' ";
                msg += "programming language specified in the";
                msg += " '" + componentEntity.name + "' ";
                msg += "component.";

                throw std::runtime_error(msg.c_str());
            }

            component->setPath(boost::filesystem::path{packageEntity.modulePaths[i]});

            std::vector<boost::filesystem::path> includePaths;
            std::vector<boost::filesystem::path> sourcePaths;

            for (const ComponentSourceEntity &moduleSourceEntity : componentEntity.sources) {
                if (moduleSourceEntity.public_) {
                    includePaths.push_back(moduleSourceEntity.path);
                } else {
                    sourcePaths.push_back(moduleSourceEntity.path);
                }
            }

            component->setIncludePaths(includePaths);
            component->setSourcePaths(sourcePaths);
        }

        // solve component dependencies
        std::vector<Module*> modules = package->getModules();

        for (int i=0; i<moduleEntities.size(); i++) {
            const ComponentEntity &componentEntity = moduleEntities[i];
            Module *component = modules[i];

            for (const std::string dependency : componentEntity.dependencies) {
                // Solve the dependency with the modules inside the package
                bool found = false;
                for (const Module *dependentModule : modules) {
                    if (dependency == dependentModule->getName()) {
                        auto dependencies = component->getDependencies();
                        dependencies.push_back(dependentModule);
                        component->setDependencies(dependencies);

                        found = true;
                        break;
                    }
                }

                // Solve the dependency as a external one
                if (! found && packageRegistry) {
                    auto dependentModule = packageRegistry->findModule(dependency);

                    if (dependentModule) {
                        auto dependencies = component->getDependencies();
                        dependencies.push_back(dependentModule);
                        component->setDependencies(dependencies);

                        found = true;
                    }
                }

                if (! found) {
                    std::string msg = "";

                    msg += "Required dependency ";
                    msg += dependency;
                    msg += " for component ";
                    msg += component->getName();
                    msg += " couldn't be found.";

                    throw std::runtime_error(msg);
                }
            }
        }

        return package;
    }


    PackageEntity PackageFactory_FS::loadPackageEntity(const boost::filesystem::path &packagePath) const {
        const auto packageFilePath = packagePath / BOK_PACKAGE_DEFINITION_FILENAME;

        if (! checkValidBorcFile(packageFilePath)) {
            throw std::runtime_error("There is no package build file on the folder '" + packageFilePath.string() + "'");
        }

        auto packageJsonContent = fileService->load(packageFilePath.string());
        auto packageJson = nlohmann::json::parse(packageJsonContent);

        return Decoder<JsonModel, PackageEntity>{packageJson}.decode();
    }


    std::vector<ComponentEntity> PackageFactory_FS::loadModuleEntities(const boost::filesystem::path &packagePath, const PackageEntity &packageEntity) const {
        std::vector<ComponentEntity> moduleEntities;

        for (const std::string &modulePartialPath : packageEntity.modulePaths) {
            const boost::filesystem::path moduleFilePath = packagePath / modulePartialPath / BOK_COMPONENT_DEFINITION_FILENAME;

            if (! checkValidBorcFile(moduleFilePath)) {
                throw std::runtime_error("There is no component build file on this folder '" + (packagePath / modulePartialPath).string() + "'");
            }

            auto moduleJsonContent = fileService->load(moduleFilePath.string());
            auto moduleJson = nlohmann::json::parse(moduleJsonContent);

            ComponentEntity componentEntity = Decoder<JsonModel, ComponentEntity>{moduleJson}.decode();

            moduleEntities.push_back(componentEntity);
        }

        return moduleEntities;
    }


    bool PackageFactory_FS::checkValidBorcFile(const boost::filesystem::path &filePath) const {
        return !boost::filesystem::is_directory(filePath) && boost::filesystem::exists(filePath);
    }
}
