
#ifndef __BORC_PARSING_JSONDESERIALIZER_HPP__
#define __BORC_PARSING_JSONDESERIALIZER_HPP__

#include <map>
#include <vector>
#include <set>
#include <stdexcept>
#include <typeinfo>
#include <boost/hana.hpp>
#include <nlohmann/json.hpp>

#include "JSONCommon.hpp"

namespace borc {
    /**
     * @brief Deserializes the supplied JSON array into a vector of boost.hana structure values
     */
    template<typename Type>
    void deserialize(std::vector<Type> &values, const nlohmann::json &model) {
        values.resize(model.size());

        for (int i=0; i<model.size(); i++) {
            if constexpr (IsSimple<Type>::value) {
                values[i] = model[i].template get<Type>();
            } else {
                deserialize(values[i], model[i]);
            }
        }
    }


    /**
     * @brief Deserializes the supplied JSON array into a map of values
     */
    template<typename Type>
    void deserialize(std::map<std::string, Type> &values, const nlohmann::json &model) {
        for (auto& pair : values) {
            if constexpr (IsSimple<Type>::value) {
                pair.second = model[pair.first].template get<Type>();
            } else {
                deserialize(pair.second, model[pair.first]);
            }
        }
    }

    /**
     * @brief Deserializes the supplied JSON array into a vector of boost.hana structure values
     */
    template<typename Type>
    void deserialize(std::set<Type> &values, const nlohmann::json &model) {
        for (int i=0; i<model.size(); i++) {
            if constexpr (IsSimple<Type>::value) {
                values.insert(model[i].template get<Type>());
            } else {
                Type subvalue;

                deserialize(subvalue, model[i]);

                values.insert(subvalue);
            }
        }
    }

    /**
     * @brief Deserielizes the supplied JSON object into a boost.hana structure value
     */    
    template<typename Entity>
    void deserialize(Entity &entity, const nlohmann::json &model) {
        if (model.is_object()) {
            boost::hana::for_each(boost::hana::accessors<Entity>(), [&](auto pair) {
                auto fieldName = boost::hana::to<const char*>(boost::hana::first(pair));
                auto fieldValue = boost::hana::second(pair)(entity);

                typedef decltype(fieldValue) Type;

                if (const auto it = model.find(fieldName); it != model.end()) {
                    // check if current property is a simple type or a string one...
                    if constexpr (IsSimple<Type>::value) {
                        boost::hana::second(pair)(entity) = model[fieldName].template get<Type>();
                    } else {
                        deserialize(boost::hana::second(pair)(entity), model[fieldName]);
                    }
                } else {
                    // Property fieldName wasn't found in the JSON.
                    // TODO: Define what to do based as needed,
                }
            });
        } else {
            using Type = typename Entity::DefaultType;

            if constexpr (! std::is_same<Type, void>::value) {
                entity = Entity( (model.template get<Type>()) );
            } else {
                std::string msg =
                    "Don't know how to deserialize the entity, because is represented by a single value, and the entity doesn't have a default property to use that single value";

                throw std::runtime_error(msg.c_str());
            }
        }
    }
}

#endif
