
#include "InitController.hpp"
#include <iostream>

#include <boost/filesystem.hpp>
#include <borc/model/Version.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/ModuleEntity.hpp>
#include <borc/services/FileServiceImpl.hpp>

namespace borc {
    InitController::~InitController() {}

    void InitController::perform(const InitControllerOptions &options) {
        if (options.showHelp) {
            std::cout << options.helpMessage;
            return;
        }

        const auto packagePath = options.path / options.packageName;

        if (boost::filesystem::exists(packagePath / "package.borc.json")) {
            throw std::runtime_error("A package already exists in the folder '" + packagePath.string() + "'");
        }

        ModuleEntity moduleEntity;
        moduleEntity.name = options.packageName;
        moduleEntity.type = options.moduleType;
        moduleEntity.version = {1, 0, 0};
        moduleEntity.sources = {
            ModuleSourceEntity{"src", false},
            // ModuleSourceEntity{"include", true}, // TODO: Consider multiple package structures per module type
        };

        std::vector<ModuleEntity> moduleEntities = { moduleEntity };

        PackageEntity packageEntity;
        packageEntity.name = options.packageName;
        packageEntity.description = "TODO: Add the description of the " + options.packageName + " package.";
        packageEntity.modules = {
            "modules/" + moduleEntity.name
        };

        // create the package structure
        boost::filesystem::create_directories(packagePath);
        for (const auto &moduleEntity : moduleEntities) {
            const boost::filesystem::path modulePath = packagePath / "modules" / moduleEntity.name;

            boost::filesystem::create_directories(modulePath);

            // TODO: Save the JSON files!
            // save the file

            for (const auto sourcePath : moduleEntity.sources) {
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
