
#include "CliControllerImpl.hpp"

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <nlohmann/json.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/parsing/JSONDeserializer.hpp>
#include <borc/entity/Package.hpp>
#include <borc/entity/Language.hpp>
#include <borc/entity/Module.hpp>

namespace borc {
    CliControllerImpl::~CliControllerImpl() {}

    void CliControllerImpl::dispatch(const std::string &subcommand, const std::vector<std::string> &args) {
        if (subcommand == "configure") {
            if (const auto it =std::find(args.begin(), args.end(), "--toolchain"); it==args.end()) {
                throw std::runtime_error("There is no toolchain selected. Please specify one");
            }

            this->configure();
        }
    }

    std::set<std::string> CliControllerImpl::enumerateSubcommands() const {
        return { "configure" };
    }

    void CliControllerImpl::configure() {
        const auto baseFolderPath = boost::filesystem::current_path();
        const auto packageFilePath = baseFolderPath / "package.borc";

        if (boost::filesystem::is_directory(packageFilePath)) {
            throw std::runtime_error("There is no package build file on this folder.");
        }

        auto service = FileServiceImpl{};
        auto packageJsonContent = service.load(packageFilePath.string());
        auto packageJson = nlohmann::json::parse(packageJsonContent);

        Package package;
        deserialize(package, packageJson);

        std::vector<Module> modules;

        for (const std::string &modulePartialPath : package.modules) {
            const boost::filesystem::path moduleFilePath = baseFolderPath / modulePartialPath / "module.borc";

            if (boost::filesystem::is_directory(moduleFilePath)) {
                throw std::runtime_error("There is no module build file on this folder.");
            }

            auto moduleJsonContent = service.load(moduleFilePath.string());
            auto moduleJson = nlohmann::json::parse(moduleJsonContent);

            Module module;
            deserialize(module, moduleJson);

            modules.push_back(module);
        }

        // we have all the information needed in order to process the issues.
        std::set<std::string> languages;

        for (const Module &module : modules) {
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
}
