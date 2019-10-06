
#ifndef __BORC_PARSING_JSONSERIALIZER_HPP__
#define __BORC_PARSING_JSONSERIALIZER_HPP__

#include <boost/hana.hpp>
#include <nlohmann/json.hpp>

#include "JSONCommon.hpp"

namespace borc {
    /**
     * @brief Serializes the supplied JSON array into a vector of boost.hana structure values
     */
    template<typename Type>
    void serialize(nlohmann::json &model, const std::vector<Type> &values) {
        for (std::size_t i=0; i<values.size(); i++) {

        }


        values.resize(model.size());

        for (int i=0; i<model.size(); i++) {
            if constexpr (! std::is_class<Type>::value || std::is_same<Type, std::string>::value) {
                values[i] = model[i].template get<Type>();
            } else {
                deserialize(values[i], model[i]);
            }
        }
    }


    template<typename Entity, typename Model>
    Model serialize(const Entity &entity) {
        constexpr auto propertyCount = std::tuple_size<decltype(Entity::properties)>::value;
        
        Model model;

        for_sequence(std::make_index_sequence<propertyCount>{}, [&](auto i) {
            constexpr auto property = std::get<i>(Entity::properties);
            using Type = typename decltype(property)::Type;

            if constexpr (! std::is_class<Type>::value || std::is_same<Type, std::string>::value) {
                model[property.name] = entity.*(property.member);
            } else {
                model[property.name] = serialize<Type>(entity.*(property.member));
            }
        });
        
        return model;
    }
}

#endif
