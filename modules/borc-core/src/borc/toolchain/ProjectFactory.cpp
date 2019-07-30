
#include "ProjectFactory.hpp"

#include <optional>
#include <iostream>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <borc/core/FileUtil.hpp>
#include <borc/core/Module.hpp>
#include <borc/core/Project.hpp>

namespace borc {
    struct ProjectModel {
        std::string name;
        std::string description;
        std::string version;
        std::string author;
        std::string license;
        std::vector<std::string> modules;

        static ProjectModel fromJSON(const nlohmann::json &json) {
            return {
                json["name"].get<std::string>(),
                json["description"].get<std::string>(),
                json["version"].get<std::string>(),
                json["author"].get<std::string>(),
                json["license"].get<std::string>(),
                json["modules"].get<std::vector<std::string>>()
            };
        }
    };

    struct ModuleModel {
        struct Structure {
            std::map<std::string, std::string> defines;
            std::vector<std::string> imports;
            std::vector<std::string> sources;

            static Structure fromJSON(const nlohmann::json &json) {
                return {
                    json["defines"].get<std::map<std::string, std::string>>(),
                    json["imports"].get<std::vector<std::string>>(),
                    json["sources"].get<std::vector<std::string>>()
                };
            }
        };

        struct Export {
            std::vector<std::string> includePaths;

            static Export fromJSON(const nlohmann::json &json) {
                return {
                    json["includePaths"].get<std::vector<std::string>>()
                };
            }
        };

        std::string name;
        std::string type;
        Structure structure;
        std::optional<Export> export_;

        static ModuleModel fromJSON(const nlohmann::json &json) {
            auto exportIt = json.find("export");

            return {
                json["name"].get<std::string>(),
                json["type"].get<std::string>(),
                Structure::fromJSON(json["structure"]),
                exportIt != json.end() ? Export::fromJSON(*exportIt) : std::optional<Export>()
            };
        }
    };

    static nlohmann::json jsonFromFile(const std::string &filePath) {
        const std::string jsonContent = FileUtil::load(filePath);
        const nlohmann::json json = nlohmann::json::parse(jsonContent);

        return json;
    }

    static boost::filesystem::path computeProjectBasePath(const std::string &projectPath) {
        return boost::filesystem::path(projectPath).parent_path();
    }

    std::unique_ptr<Project> ProjectFactory::createProject(const std::string &filePath) const {
        std::cout << filePath << std::endl;

        const auto basePath = computeProjectBasePath(filePath);
        const auto json = jsonFromFile(filePath);
        const auto projectModel = ProjectModel::fromJSON(json);

        for (const auto &modulePartialPath : projectModel.modules) {
            const auto modulePath = basePath / modulePartialPath / "Module.borc.json";
            const auto moduleJson = jsonFromFile(modulePath.string());
            const auto moduleModel = ModuleModel::fromJSON(moduleJson);
        }

        return std::unique_ptr<Project>();
    }
}
