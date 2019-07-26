
#ifndef __BORC_ENTITY_LANGUAGE_HPP__
#define __BORC_ENTITY_LANGUAGE_HPP__

#include <string>
#include <vector>

#include <borc/utility/MetaProperty.hpp>

namespace borc {
    struct Language {
        std::string name;
        std::vector<std::string> dialects;

        constexpr static auto properties = std::make_tuple (
            property(&Language::name, "name"),
            property(&Language::dialects, "dialects")
        );

        typedef void DefaultType;
        constexpr static const char* Name = "Language";
    };
}

#endif

