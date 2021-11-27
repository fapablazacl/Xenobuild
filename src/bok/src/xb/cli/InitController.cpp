
#include "InitController.hpp"
#include <iostream>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <bok/core/Constants.hpp>
#include <bok/core/Version.hpp>
#include <bok/core/FileService_FS.hpp>
#include <bok/core/io/PackageEntity.hpp>
#include <bok/core/io/ComponentEntity.hpp>
#include <bok/core/io/JsonModel.hpp>
#include <bok/core/io/Encoder.hpp>

namespace bok {
    struct InitControllerOptions {
        bool showHelp = false;
        std::string helpMessage;

        boost::filesystem::path path;
        std::string packageName;
        std::string moduleType;
    };


    static InitControllerOptions parse(int argc, char **argv) {
        boost::program_options::options_description desc("Allowed options for Configure subcommand");

        desc.add_options()
            ("help", "produce this message")
            ("path", boost::program_options::value<std::string>(), "Configuration path. Defaults to the current folder")
            ("name", boost::program_options::value<std::string>(), "The package name to be created. By default")
            ("type", boost::program_options::value<std::string>(), "The default component type to be created. Can be application/cli, application/gui, library/static or library/dynamic")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        InitControllerOptions options;

        if (vm.count("help")) {
            std::stringstream ss;

            ss << desc << "\n";

            options.showHelp = true;
            options.helpMessage = ss.str();
        }
        
        if (vm.count("path")) {
            options.path = vm["path"].as<boost::filesystem::path>();
        } else {
            options.path = boost::filesystem::current_path();
        }

        if (vm.count("name")) {
            options.packageName = vm["name"].as<std::string>();
        }

        if (vm.count("type")) {
            options.moduleType = vm["type"].as<std::string>();
        }

        return options;
    }
}


namespace bok {
    InitController::~InitController() {}

    void InitController::perform(int argc, char **argv) {
        const auto options = parse(argc, argv);

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
            // ComponentSourceEntity{"include", true}, // TODO: Consider multiple package structures per component type
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

            // save component to file
            nlohmann::json moduleJson = Encoder<JsonModel, ComponentEntity>{componentEntity}.encode();
            std::ofstream moduleFileStream((modulePath / "component.bok.json").string());
            moduleFileStream << std::setw(4) << moduleJson << std::endl;

            for (const auto sourcePath : componentEntity.sources) {
                boost::filesystem::create_directories(modulePath / sourcePath.path);

                // create the sample file
                const auto sourceFilePath = modulePath / sourcePath.path / "Main.cpp";

                auto fileService = FileService_FS{};

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
