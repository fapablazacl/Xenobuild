
#ifndef __BORC_PARSING_JSONDESERIALIZER_HPP__
#define __BORC_PARSING_JSONDESERIALIZER_HPP__

#include <vector>
#include <stdexcept>
#include <typeinfo>
#include <borc/utility/MetaProperty.hpp>

namespace borc {
    template<typename Type, typename Model>
    void deserialize(std::vector<Type> &values, const Model &model) {
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
    void deserialize(Entity &entity, const Model &model) {
        constexpr auto propertyCount = std::tuple_size<decltype(Entity::properties)>::value;

        if (model.is_object()) {
            for_sequence(std::make_index_sequence<propertyCount>{}, [&](auto i) {
                constexpr auto property = std::get<i>(Entity::properties);
                using Type = typename decltype(property)::Type;

                if (const auto propertyIt = model.find(property.name); propertyIt != model.end()) {
                    if constexpr (! std::is_class<Type>::value || std::is_same<Type, std::string>::value) {
                        entity.*(property.member) = model[property.name].template get<Type>();
                    } else {
                        // std::cout << "Deserializing " << property.name << " property" << std::endl;
                        deserialize(entity.*(property.member), model[property.name]);
                    }
                } else {
                    // property not found. Maybe raise an exception for non-optional types?
                    // std::cout << "DEBUG: " << "The '" << property.name << "' property wasn't found" << std::endl;
                }
            });
        } else {
            using Type = typename Entity::DefaultType;

            if constexpr (! std::is_same<Type, void>::value) {
                entity = Entity( (model.template get<Type>()) );
            } else {
                std::string msg =
                    "Don't know how to deserialize the " + 
                    std::string(Entity::Name) + 
                    " entity, because is represented by a single value, and the entity doesn't have a default property to use that single value";

                throw std::runtime_error(msg.c_str());
            }
        }
    }
}

#endif
