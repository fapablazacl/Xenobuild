
#ifndef __BORC_ENTITY_PACKAGE_HPP__
#define __BORC_ENTITY_PACKAGE_HPP__

#include <string>
#include <vector>

#include "Language.hpp"
#include "Module.hpp"

namespace borc {
    struct Package {
        std::string name;
        std::string description;
        Language language;
        std::vector<std::string> modules;
        std::vector<std::string> packageSearchPaths;

        constexpr static auto properties = std::make_tuple (
            property(&Package::name, "name"),
            property(&Package::description, "description"),
            property(&Package::language, "language"),
            property(&Package::modules, "modules"),
            property(&Package::packageSearchPaths, "packageSearchPaths")
        );

        typedef void DefaultType;

        constexpr static const char* Name = "Package";
    };
}
#endif
