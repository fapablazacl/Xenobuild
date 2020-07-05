
#include "BuildController2.hpp"

#include <iostream>
#include <cassert>
#include <yaml-cpp/yaml.h>


namespace bok {
    void BuildController2::perform(int argc, char **argv) {
        auto package = parse("package.yaml");

        assert(package.name == "01-hello-world");
        assert(package.components.size() == 1);
        assert(package.components[0].name == "hello-world");
        assert(package.components[0].type == "app/cli");
        assert(package.components[0].language == "c++/17");
        assert(package.components[0].sources.size() == 1);
        assert(package.components[0].sources[0] == "./*");
    }

    BuildController2::PackageIO BuildController2::parse(const std::string &packageFile) const {
        YAML::Node packageNode = YAML::LoadFile(packageFile);

        BuildController2::PackageIO package;
        package.name = packageNode["name"].as<std::string>();

/*
        for (YAML::Node componentNode : packageNode["components"]) {
            BuildController2::ComponentIO component;

            component.name = componentNode["name"].as<std::string>();
            component.type = componentNode["type"].as<std::string>();
            component.language = componentNode["language"].as<std::string>();
            component.sources = componentNode["sources"].as<std::vector<std::string>>();

            package.components.push_back(component);
        }*/

        return package;
    }
}
