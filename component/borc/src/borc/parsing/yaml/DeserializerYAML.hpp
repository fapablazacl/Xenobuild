
#ifndef __BORC_PARSING_DESERIALIZERJSON_HPP__
#define __BORC_PARSING_DESERIALIZERJSON_HPP__

#include <map>
#include <vector>
#include <set>
#include <stdexcept>
#include <typeinfo>
#include <boost/hana.hpp>
#include <yaml-cpp/yaml.h>

#include <borc/parsing/Common.hpp>

namespace borc {
    /**
     * @brief Deserializes the supplied JSON array into a vector of boost.hana structure values
     */
    template<typename T>
    void deserialize(std::vector<T> &values, const YAML::Node &node) {
        values.resize(0);

        int i = 0;

        for (const auto &child : node) {
            if constexpr (IsSimple<T>::value) {
                values.push_back( child.template as<T>() );
                
            } else {
                values.push_back({});
                deserialize(values[i], child);
            }

            i++;
        }
    }


    /**
     * @brief Deserializes the supplied JSON array into a map of values
     */
    template<typename T>
    void deserialize(std::map<std::string, T> &values, const YAML::Node &node) {
        for (auto& pair : values) {
            if constexpr (IsSimple<T>::value) {
                pair.second = node[pair.first].template as<T>();
            } else {
                deserialize(pair.second, node[pair.first]);
            }
        }
    }


    /**
     * @brief Deserializes the supplied JSON array into a vector of boost.hana structure values
     */
    template<typename T>
    void deserialize(std::set<T> &values, const YAML::Node &node) {
        for (int i=0; i<node.size(); i++) {
            if constexpr (IsSimple<T>::value) {
                values.insert(node[i].template as<T>());
            } else {
                T subvalue;

                deserialize(subvalue, node[i]);

                values.insert(subvalue);
            }
        }
    }


    /**
     * @brief Deserielizes the supplied JSON object into a boost.hana structure value
     */    
    template<typename Entity>
    void deserialize(Entity &entity, const YAML::Node &node) {
        if (node.IsDefined()) {
            boost::hana::for_each(boost::hana::accessors<Entity>(), [&](auto pair) {
                auto fieldName = boost::hana::to<const char*>(boost::hana::first(pair));
                auto fieldValue = boost::hana::second(pair)(entity);

                typedef decltype(fieldValue) Type;

                if (node.IsDefined(fieldName)) {
                    // check if current property is a simple type or a string one...
                    if constexpr (IsSimple<Type>::value) {
                        boost::hana::second(pair)(entity) = node[fieldName].template as<Type>();
                    } else {
                        deserialize(boost::hana::second(pair)(entity), node[fieldName]);
                    }
                } else {
                    // Property fieldName wasn't found in the JSON.
                    // TODO: Define what to do based as needed,
                }
            });
        } else {
            using Type = typename Entity::DefaultType;

            if constexpr (! std::is_same<Type, void>::value) {
                entity = Entity( (node.template as<Type>()) );
            } else {
                std::string msg =
                    "Don't know how to deserialize the entity, because is represented by a single value, and the entity doesn't have a default property to use that single value";

                throw std::runtime_error(msg.c_str());
            }
        }
    }
}

#endif
