
#ifndef __BORC_ENTITY_PACKAGE_HPP__
#define __BORC_ENTITY_PACKAGE_HPP__

#include <string>
#include <vector>

#include "LanguageEntity.hpp"
#include "ModuleEntity.hpp"

namespace borc {
    struct PackageEntity {
        std::string name;
        std::string description;
        LanguageEntity language;
        std::vector<std::string> modules;
        std::vector<std::string> packageSearchPaths;

        constexpr static auto properties = std::make_tuple (
            property(&PackageEntity::name, "name"),
            property(&PackageEntity::description, "description"),
            property(&PackageEntity::language, "language"),
            property(&PackageEntity::modules, "modules"),
            property(&PackageEntity::packageSearchPaths, "packageSearchPaths")
        );

        typedef void DefaultType;

        constexpr static const char* Name = "Package";
    };
}
#endif
