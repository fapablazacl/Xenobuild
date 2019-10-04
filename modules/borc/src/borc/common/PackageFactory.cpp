
#include "PackageFactory.hpp"

#include <iostream>
#include <map>
#include <borc/model/Package.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/ModuleEntity.hpp>


namespace borc {
    std::unique_ptr<Package> PackageFactory::createPackage(const PackageEntity &packageEntity, const std::vector<ModuleEntity> &moduleEntities) {
        // now we are ready to create the package and artifacts instances
        auto package = std::make_unique<Package>(packageEntity.name);

        // available artifact types for C/C++ projects
        const std::map<std::string, Artifact::Type> artifactTypeMap = {
            {"application/cli", Artifact::Type{"application", "cli"} },
            {"application/gui", Artifact::Type{"application", "gui"} },
            {"library/static", Artifact::Type{"library", "static"} },
            {"library/dynamic", Artifact::Type{"library", "dynamic"} }
        };

        for (int i=0; i<moduleEntities.size(); i++) {
            const ModuleEntity &moduleEntity = moduleEntities[i];

            Artifact *artifact = package->createArtifact();

            artifact->setName(moduleEntity.name);

            if (auto artifactTypeIt = artifactTypeMap.find(moduleEntity.type); artifactTypeIt != artifactTypeMap.end()) {
                artifact->setType(artifactTypeIt->second);
            } else {
                std::string msg;

                msg += "Invalid artifact type";
                msg += " '" + moduleEntity.type + "' ";
                msg += "for the";
                msg += " '" + moduleEntity.language + "' ";
                msg += "programming language specified in the";
                msg += " '" + moduleEntity.name + "' ";
                msg += "artifact.";

                throw std::runtime_error(msg.c_str());
            }

            artifact->setPath(boost::filesystem::path{packageEntity.modules[i]});

            std::vector<boost::filesystem::path> includePaths;
            std::vector<boost::filesystem::path> sourcePaths;

            for (const ModuleSourceEntity &moduleSourceEntity : moduleEntity.sources) {
                if (moduleSourceEntity.public_) {
                    includePaths.push_back(moduleSourceEntity.path);
                } else {
                    sourcePaths.push_back(moduleSourceEntity.path);
                }
            }

            artifact->setIncludePaths(includePaths);
            artifact->setSourcePaths(sourcePaths);
        }

        // solve module dependencies
        std::vector<Artifact*> artifacts = package->getArtifacts();

        for (int i=0; i<moduleEntities.size(); i++) {
            const ModuleEntity &moduleEntity = moduleEntities[i];
            Artifact *artifact = artifacts[i];

            for (const std::string dependency :  moduleEntity.dependencies) {
                // TODO: Expand the dependency solving from the (future) context object ...
                bool found = false;
                for (const Artifact *dependentArtifact : artifacts) {
                    if (dependency == dependentArtifact->getName()) {
                        auto dependencies = artifact->getDependencies();
                        dependencies.push_back(dependentArtifact);
                        artifact->setDependencies(dependencies);

                        found = true;
                        break;
                    }
                }

                if (! found) {
                    std::cout << "WARNING: dependency " << dependency << " for artifact " << artifact->getName() << " couldn't be found" << std::endl;
                }
            }
        }

        return package;
    }
}
