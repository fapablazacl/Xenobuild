
#pragma once 

#include <nlohmann/json.hpp>

namespace borc {
    typedef nlohmann::json JsonModel;

    bool model_exist_key(const JsonModel &model, const std::string key) {
        if (const auto it = model.find(key); it != model.end()) {
            return true;
        }

        return false;
    }


    template<typename T>
    T model_get(const JsonModel &model) {
        return model.template get<T>();
    }


    bool model_is_object(const JsonModel &model) {
        return model.is_object();
    }
}
