
#ifndef __BORC_ENTITY_LANGUAGEENTITY_HPP__
#define __BORC_ENTITY_LANGUAGEENTITY_HPP__

#include <string>
#include <vector>

#include <boost/hana.hpp>

namespace borc {
    struct LanguageEntity {
        std::string name;
        std::vector<std::string> dialects;

        typedef void DefaultType;
    };
}

BOOST_HANA_ADAPT_STRUCT(borc::LanguageEntity, name, dialects);

#endif
