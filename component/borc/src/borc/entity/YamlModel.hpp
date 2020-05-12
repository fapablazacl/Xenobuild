
#pragma once 

#include <yaml-cpp/yaml.h>

namespace borc {
    typedef YAML::Node YamlModel;

    inline bool model_exist_key(const YamlModel &model, const std::string key) {
        return model[key].IsDefined();
    }

    template<typename T>
    inline T model_get(const YamlModel &model) {
        return model.template as<T>();
    }

    inline bool model_is_object(const YamlModel &model) {
        return model.IsMap();
    }
}
