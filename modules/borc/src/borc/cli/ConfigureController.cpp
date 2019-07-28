
#include "ConfigureController.hpp"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <nlohmann/json.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/parsing/JSONDeserializer.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/LanguageEntity.hpp>
#include <borc/entity/ModuleEntity.hpp>

#include <borc/model/Artifact.hpp>
#include <borc/model/Package.hpp>

namespace borc {
    ConfigureController::~ConfigureController() {}

    void ConfigureController::perform(int argc, char **argv) {
        // parse the command line
        boost::program_options::options_description desc("Allowed options for Configure subcommand");
        desc.add_options()
            ("help", "produce help message")
            ("build-type", boost::program_options::value<std::string>(), "set build type (debug, release, all)")
            ("toolchain", boost::program_options::value<std::string>(), "set toolchain (gcc, vc, clang)")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);    

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return;
        }

        if (vm.count("build-type")) {
            std::cout << "Build type was set to " << vm["build-type"].as<std::string>() << ".\n";
        } else {
            std::cout << "Build type defaulted to 'all'.\n";
        }

        // perform the configure operation
        const auto baseFolderPath = boost::filesystem::current_path();
        const auto packageFilePath = baseFolderPath / "package.borc";

        if (! checkValidBorcFile(packageFilePath)) {
            throw std::runtime_error("There is no package build file on the folder '" + packageFilePath.string() + "'");
        }

        auto service = FileServiceImpl{};
        auto packageJsonContent = service.load(packageFilePath.string());
        auto packageJson = nlohmann::json::parse(packageJsonContent);

        PackageEntity packageEntity;
        deserialize(packageEntity, packageJson);

        std::vector<ModuleEntity> moduleEntities;

        for (const std::string &modulePartialPath : packageEntity.modules) {
            const boost::filesystem::path moduleFilePath = baseFolderPath / modulePartialPath / "module.borc";

            if (! checkValidBorcFile(moduleFilePath)) {
                throw std::runtime_error("There is no module build file on this folder '" + moduleFilePath.string() + "'");
            }

            auto moduleJsonContent = service.load(moduleFilePath.string());
            auto moduleJson = nlohmann::json::parse(moduleJsonContent);

            ModuleEntity moduleEntity;
            deserialize(moduleEntity, moduleJson);

            moduleEntities.push_back(moduleEntity);
        }

        // we have all the information needed in order to process the packages.
        std::set<std::string> languages;

        for (const ModuleEntity &moduleEntity : moduleEntities) {
            languages.insert(moduleEntity.language);
        }

        std::cout << "Detected programming languages:" << std::endl;
        for (const std::string &lang : languages) {
            std::cout << "    " << lang << std::endl;
        }

        // with the programming languages, we can issue to the user what toolchains must be configured!
        std::map<std::string, std::string> toolchains = {
            {"c++/17", "gcc"},
            {"c++/17", "clang"}
        };

        // now we are ready to create the package and artifacts instances
        auto package = std::make_unique<Package>(packageEntity.name);

        // available artifact types for C/C++ projects
        const std::map<std::string, Artifact::Type> artifactTypeMap = {
            {"application/cli", Artifact::Type::ApplicationCli},
            {"application/cli", Artifact::Type::ApplicationGui},
            {"library/static", Artifact::Type::LibraryStatic},
            {"library/dynamic", Artifact::Type::LibraryDynamic}
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
    }


    bool ConfigureController::checkValidBorcFile(const boost::filesystem::path &filePath) const {
        return !boost::filesystem::is_directory(filePath) && boost::filesystem::exists(filePath);
    }
}
