
#ifndef __BORC_ENTITY_PACKAGE_HPP__
#define __BORC_ENTITY_PACKAGE_HPP__

#include <string>
#include <vector>

#include "LanguageEntity.hpp"
#include "ModuleEntity.hpp"

#include <boost/hana.hpp>

namespace borc {
    struct PackageEntity {
        std::string name;
        std::string description;
        LanguageEntity language;
        std::vector<std::string> modulePaths;
        std::vector<std::string> packageSearchPaths;

        typedef void DefaultType;
    };
}

BOOST_HANA_ADAPT_STRUCT(borc::PackageEntity, name, description, language, modulePaths, packageSearchPaths);

#endif
