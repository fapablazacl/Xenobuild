
#ifndef __BORC_PARSING_JSONDESERIALIZER_HPP__
#define __BORC_PARSING_JSONDESERIALIZER_HPP__

#include <vector>
#include <stdexcept>
#include <typeinfo>
#include <boost/hana.hpp>
#include <nlohmann/json.hpp>

namespace borc {
    /**
     * @brief Deserializes the supplied JSON array into a vector of boost.hana structure values
     */
    template<typename Type>
    void deserialize(std::vector<Type> &values, const nlohmann::json &model) {
        values.resize(model.size());

        for (int i=0; i<model.size(); i++) {
            if constexpr (! std::is_class<Type>::value || std::is_same<Type, std::string>::value) {
                values[i] = model[i].template get<Type>();
            } else {
                deserialize(values[i], model[i]);
            }
        }
    }

    /**
     * @brief Deserielizes the supplied JSON object into a boost.hana structure value
     */    
    template<typename Entity>
    void deserialize(Entity &entity, const nlohmann::json &model) {
        if (model.is_object()) {
            boost::hana::for_each(entity, [&](auto pair) {
                auto fieldName = boost::hana::to<const char*>(boost::hana::first(pair));
                auto fieldValue = boost::hana::second(pair);

                if (const auto it = model.find(fieldName); it != model.end()) {
                    

                    return;
                } else {
                    // Property fieldName wasn't found in the JSON.
                }
            });

            /*
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
            */
        } else {
            /*
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
            */
        }
    }
}

#endif
