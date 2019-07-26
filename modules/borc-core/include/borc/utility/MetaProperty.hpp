
#ifndef __BORC_UTILITY_METAPROPERTY_HPP__
#define __BORC_UTILITY_METAPROPERTY_HPP__

#include <utility>
#include <type_traits>

#include <iostream>

namespace borc {
    struct no_property_t {};

    template<typename Class, typename T>
    struct MetaProperty {
        constexpr MetaProperty(T Class::*pmember, const char *pname)
            : member{pmember}, name{pname} {}
        
        using Type = T;
        
        T Class::*member;
        const char *name;
    };
    
    template<typename Class, typename T>
    constexpr auto property(T Class::*member, const char *name) {
        return MetaProperty{member, name};
    }
    
    template<typename T, T... S, typename F>
    constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
        (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
    }

}

#endif
