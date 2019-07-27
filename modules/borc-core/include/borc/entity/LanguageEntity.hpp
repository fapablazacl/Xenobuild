
#ifndef __BORC_ENTITY_LANGUAGEENTITY_HPP__
#define __BORC_ENTITY_LANGUAGEENTITY_HPP__

#include <string>
#include <vector>

#include <borc/utility/MetaProperty.hpp>

namespace borc {
    struct LanguageEntity {
        std::string name;
        std::vector<std::string> dialects;

        constexpr static auto properties = std::make_tuple (
            property(&LanguageEntity::name, "name"),
            property(&LanguageEntity::dialects, "dialects")
        );

        typedef void DefaultType;
        constexpr static const char* Name = "Language";
    };
}

#endif

