
#ifndef __BOK_ENTITY_LANGUAGEENTITY_HPP__
#define __BOK_ENTITY_LANGUAGEENTITY_HPP__

#include <string>
#include <vector>

#include <boost/hana.hpp>

namespace bok {
    struct LanguageEntity {
        std::string name;
        std::vector<std::string> dialects;

        typedef void DefaultType;
    };
}

BOOST_HANA_ADAPT_STRUCT(bok::LanguageEntity, name, dialects);

#endif
