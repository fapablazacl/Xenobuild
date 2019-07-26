
#ifndef __BORC_PARSING_JSONSERIALIZER_HPP__
#define __BORC_PARSING_JSONSERIALIZER_HPP__

#error Header not ready for use yet. Please check later! :)

namespace borc {
#if 0
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
#endif
}

#endif
