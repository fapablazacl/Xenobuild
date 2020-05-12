
#pragma once 

#include <typeinfo>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <stdexcept>
#include <typeinfo>
#include <boost/hana.hpp>


namespace borc {
    template<typename Model, typename Entity>
    class Encoder {
    public:
        explicit Encoder(Entity entity_) : entity(entity_) {}


        Model serialize() const {
            Model model;

            serialize(model, entity);

            return model;
        }
        
    protected:
        /**
         * @brief Serializes the supplied boost.hana vector structure values to a JSON array
         */
        template<typename Type>
        static void serialize(Model &model, const std::vector<Type> &values) {
            for (const Type &value : values) {
                if constexpr (isSimple<Type>()) {
                    model.push_back(value);
                } else {
                    Model submodel;

                    serialize(submodel, value);

                    model.push_back(submodel);
                }
            }
        }


        template<typename Type>
        static void serialize(Model &model, const std::set<Type> &values) {
            for (const Type &value : values) {
                if constexpr (isSimple<Type>()) {
                    model.push_back(value);
                } else {
                    Model submodel;

                    serialize(submodel, value);

                    model.push_back(submodel);
                }
            }
        }


        /**
         * @brief Deserializes the supplied JSON array into a map of values
         */
        template<typename Type>
        static void serialize(Model &model, const std::map<std::string, Type> &values) {
            for (auto& pair : values) {
                if constexpr (isSimple<Type>()) {
                    model[pair.first] = pair.second;
                } else {
                    Model submodel;
                    serialize(submodel, pair.second);
                    
                    model[pair.first] = submodel;
                }
            }
        }


        /**
         * @brief Deserielizes the supplied JSON object into a boost.hana structure value
         */    
        template<typename SubEntity>
        static void serialize(Model &model, const SubEntity &entity) {
            boost::hana::for_each(boost::hana::accessors<SubEntity>(), [&](auto pair) {
                auto fieldName = boost::hana::to<const char*>(boost::hana::first(pair));
                auto fieldValue = boost::hana::second(pair)(entity);

                typedef decltype(fieldValue) Type;

                // check if current property is a simple type
                if constexpr (isSimple<Type>()) {
                    model[fieldName] = fieldValue;
                } else {
                    Model submodel;

                    serialize(submodel, fieldValue);

                    model[fieldName] = submodel;
                }
            });
        }


        template<typename T>
        static constexpr bool isSimple() {
            return !std::is_class<T>::value || std::is_same<T, std::string>::value;
        }

    protected:
        Entity entity;
    };
}
