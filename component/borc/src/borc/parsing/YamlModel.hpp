
#pragma once 

#include <yaml-cpp/yaml.h>

namespace borc {
    class YamlModel {
    public:
        YamlModel() {}
        
        YamlModel(YAML::Node node_) : node(node_) {}


    private:
        YAML::Node node;
    };
}
