
#ifndef __BORC_PARSING_JSONSERIALIZER_HPP__
#define __BORC_PARSING_JSONSERIALIZER_HPP__

#include <boost/hana.hpp>
#include <nlohmann/json.hpp>

#include "JSONCommon.hpp"

namespace borc {
    /**
     * @brief Serializes the supplied boost.hana vector structure values to a JSON array
     */
    template<typename Type>
    void serialize(nlohmann::json &model, const std::vector<Type> &values) {
        for (const Type &value : values) {
            if constexpr (IsSimple<Type>::value) {
                model.push_back(value);
            } else {
                nlohmann::json submodel;

                serialize(submodel, value);

                model.push_back(submodel);
            }
        }
    }

    /**
     * @brief Deserielizes the supplied JSON object into a boost.hana structure value
     */    
    template<typename Entity>
    void serialize(nlohmann::json &model, const Entity &entity) {
        boost::hana::for_each(boost::hana::accessors<Entity>(), [&](auto pair) {
            auto fieldName = boost::hana::to<const char*>(boost::hana::first(pair));
            auto fieldValue = boost::hana::second(pair)(entity);

            typedef decltype(fieldValue) Type;

            // check if current property is a simple type
            if constexpr (IsSimple<Type>::value) {
                model[fieldName] = fieldValue;
            } else {
                nlohmann::json submodel;

                serialize(submodel, fieldValue);

                model[fieldName] = submodel;
            }
        });
    }
}

#endif
