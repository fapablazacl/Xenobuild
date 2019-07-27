
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

        if (checkValidBorcFile(packageFilePath)) {
            throw std::runtime_error("There is no package build file on this folder.");
        }

        auto service = FileServiceImpl{};
        auto packageJsonContent = service.load(packageFilePath.string());
        auto packageJson = nlohmann::json::parse(packageJsonContent);

        PackageEntity package;
        deserialize(package, packageJson);

        std::vector<ModuleEntity> modules;

        for (const std::string &modulePartialPath : package.modules) {
            const boost::filesystem::path moduleFilePath = baseFolderPath / modulePartialPath / "module.borc";

            if (checkValidBorcFile(moduleFilePath)) {
                throw std::runtime_error("There is no module build file on this folder.");
            }

            auto moduleJsonContent = service.load(moduleFilePath.string());
            auto moduleJson = nlohmann::json::parse(moduleJsonContent);

            ModuleEntity module;
            deserialize(module, moduleJson);

            modules.push_back(module);
        }

        // we have all the information needed in order to process the issues.
        std::set<std::string> languages;

        for (const ModuleEntity &module : modules) {
            languages.insert(module.language);
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
    }


    bool ConfigureController::checkValidBorcFile(const boost::filesystem::path &filePath) const {
        return !boost::filesystem::is_directory(filePath) && boost::filesystem::exists(filePath);
    }
}
