
#include "BuildController2.hpp"

#include <iostream>
#include <cassert>
#include <yaml-cpp/yaml.h>
#include <bok/core/package/Package.hpp>
#include <bok/core/package/Module.hpp>

namespace bok {
    void BuildController2::perform(int argc, char **argv) {
        auto packageIO = parse("package.yaml");
        auto package = createPackage(packageIO);
    }


    BuildController2::PackageIO BuildController2::parse(const std::string &packageFile) const {
        YAML::Node rootNode = YAML::LoadFile(packageFile);

        if (rootNode["package"]) {
            YAML::Node packageNode = rootNode["package"];

            BuildController2::PackageIO package;

            assert(packageNode["name"]);

            package.name = packageNode["name"].as<std::string>();

            for (YAML::Node componentNode : packageNode["components"]) {
                BuildController2::ComponentIO component;

                component.name = componentNode["name"].as<std::string>();
                component.type = componentNode["type"].as<std::string>();
                component.language = componentNode["language"].as<std::string>();
                component.sources = componentNode["sources"].as<std::vector<std::string>>();

                package.components.push_back(component);
            }

            // TODO: Move the a unit test
            assert(package.name == "01-hello-world");
            assert(package.components.size() == 1);
            assert(package.components[0].name == "hello-world");
            assert(package.components[0].type == "app/cli");
            assert(package.components[0].language == "c++/17");
            assert(package.components[0].sources.size() == 1);
            assert(package.components[0].sources[0] == "*");

            return package;
        }

        return {};
    }


    std::unique_ptr<Package> BuildController2::createPackage(const BuildController2::PackageIO &data) const {
        auto package = std::make_unique<Package>(data.name);

        for (const auto &componentData : data.components) {
            auto component = package->createModule<Component>();
        }

        return package;
    }
}
