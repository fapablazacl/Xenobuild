
#ifndef __BORC_PARSING_JSONCOMMON_HPP__
#define __BORC_PARSING_JSONCOMMON_HPP__

#include <typeinfo>
#include <string>

namespace borc {
    template<typename T>
    struct IsSimple {
        static constexpr bool value = !std::is_class<T>::value || std::is_same<T, std::string>::value;
    };
}

#endif
