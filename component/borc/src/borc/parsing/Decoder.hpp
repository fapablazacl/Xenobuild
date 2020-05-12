
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
    class Decoder {
    public:
        explicit Decoder(Model model) : root(model) {}


        Entity deserialize() const {
            Entity entity;

            deserialize(entity, root);

            return entity;
        }


    protected:
        template<typename Type>
        static void deserialize(std::vector<Type> &values, const Model &model) {
            values.resize(model.size());

            for (int i=0; i<model.size(); i++) {
                if constexpr (isSimple<Type>()) {
                    values[i] = model_get<Type>(model[i]);
                } else {
                    deserialize(values[i], model[i]);
                }
            }
        }


        /**
         * @brief Deserializes the supplied model array into a map of values
         */
        template<typename Type>
        static void deserialize(std::map<std::string, Type> &values, const Model &model) {
            for (auto& pair : values) {
                if constexpr (isSimple<Type>()) {
                    pair.second = model[pair.first].template get<Type>();
                } else {
                    deserialize(pair.second, model[pair.first]);
                }
            }
        }


        /**
         * @brief Deserializes the supplied model array into a vector of boost.hana structure values
         */
        template<typename Type>
        static void deserialize(std::set<Type> &values, const Model &model) {
            for (int i=0; i<model.size(); i++) {
                if constexpr (isSimple<Type>()) {
                    values.insert(model[i].template get<Type>());
                } else {
                    Type subvalue;

                    deserialize(subvalue, model[i]);

                    values.insert(subvalue);
                }
            }
        }


        /**
         * @brief Deserielizes the supplied model object into a boost.hana structure value
         */
        template<typename SubEntity>
        static void deserialize(SubEntity &entity, const Model &model) {
            if (model_is_object(model)) {
                boost::hana::for_each(boost::hana::accessors<SubEntity>(), [&](auto pair) {
                    auto fieldName = boost::hana::to<const char*>(boost::hana::first(pair));
                    auto fieldValue = boost::hana::second(pair)(entity);

                    typedef decltype(fieldValue) Type;

                    if (model_exist_key(model, fieldName)) {
                        // check if current property is a simple type or a string one...
                        if constexpr (isSimple<Type>()) {
                            boost::hana::second(pair)(entity) = model_get<Type>(model[fieldName]);
                        } else {
                            deserialize(boost::hana::second(pair)(entity), model[fieldName]);
                        }
                    } else {
                        // Property fieldName wasn't found in the JSON.
                        // TODO: Define what to do based as needed,
                    }
                });
            } else {
                using Type = typename SubEntity::DefaultType;

                if constexpr (! std::is_same<Type, void>::value) {
                    entity = SubEntity{ (model_get<Type>(model)) };
                } else {
                    std::string msg =
                        "Don't know how to deserialize the entity, because is represented by a single value, and the entity doesn't have a default property to use that single value";

                    throw std::runtime_error(msg.c_str());
                }
            }
        }


        template<typename T>
        static constexpr bool isSimple() {
            return !std::is_class<T>::value || std::is_same<T, std::string>::value;
        }
        
    protected:
        Model root;
    };
}