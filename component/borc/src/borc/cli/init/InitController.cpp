
#include "InitController.hpp"
#include <iostream>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <borc/common/Constants.hpp>
#include <borc/model/Version.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/ComponentEntity.hpp>
#include <borc/entity/JsonModel.hpp>
#include <borc/entity/Encoder.hpp>
#include <borc/services/FileServiceImpl.hpp>

namespace bok {
    InitController::~InitController() {}

    void InitController::perform(const InitControllerOptions &options) {
        if (options.showHelp) {
            std::cout << options.helpMessage;
            return;
        }

        const auto packagePath = options.path / options.packageName;

        if (boost::filesystem::exists(packagePath / BOK_PACKAGE_DEFINITION_FILENAME)) {
            throw std::runtime_error("A package already exists in the folder '" + packagePath.string() + "'");
        }

        ComponentEntity componentEntity;
        componentEntity.name = options.packageName;
        componentEntity.type = options.moduleType;
        componentEntity.version = {1, 0, 0};
        componentEntity.sources = {
            ComponentSourceEntity{"src", false},
            // ComponentSourceEntity{"include", true}, // TODO: Consider multiple package structures per module type
        };

        std::vector<ComponentEntity> moduleEntities = { componentEntity };

        PackageEntity packageEntity;
        packageEntity.name = options.packageName;
        packageEntity.description = "TODO: Add the description of the " + options.packageName + " package.";
        packageEntity.modulePaths = {
            "modules/" + componentEntity.name
        };

        // save package to file
        nlohmann::json packageJson = Encoder<JsonModel, PackageEntity>{packageEntity}.encode();
        std::ofstream packageFileStream((packagePath / BOK_PACKAGE_DEFINITION_FILENAME).string());
        packageFileStream << std::setw(4) << packageJson << std::endl;

        // create the package structure
        boost::filesystem::create_directories(packagePath);
        for (const auto &componentEntity : moduleEntities) {
            const boost::filesystem::path modulePath = packagePath / "modules" / componentEntity.name;

            boost::filesystem::create_directories(modulePath);

            // save module to file
            nlohmann::json moduleJson = Encoder<JsonModel, ComponentEntity>{componentEntity}.encode();
            std::ofstream moduleFileStream((modulePath / "module.borc.json").string());
            moduleFileStream << std::setw(4) << moduleJson << std::endl;

            for (const auto sourcePath : componentEntity.sources) {
                boost::filesystem::create_directories(modulePath / sourcePath.path);

                // create the sample file
                const auto sourceFilePath = modulePath / sourcePath.path / "Main.cpp";

                auto fileService = FileServiceImpl{};

                fileService.save(sourceFilePath.string() , R"(
#include <iostream>

int main() {
    std::cout << "Hello, World" << std::endl;
    std::cin.get();

    return 0;
}
)");
            }
        }
    }
}
