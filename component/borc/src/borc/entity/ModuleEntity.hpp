
#ifndef __BORC_ENTITY_MODULEENTITY_HPP__
#define __BORC_ENTITY_MODULEENTITY_HPP__

#include <string>
#include <vector>

#include <boost/hana.hpp>

namespace bok {
    struct ModuleSourceEntity {
        std::string path;
        bool public_ = false;

        ModuleSourceEntity() {}

        explicit ModuleSourceEntity(const std::string &path_) : path(path_) {}

        explicit ModuleSourceEntity(const std::string &path_, const bool public__) : path(path_), public_(public__) {}

        typedef std::string DefaultType;
    };

    struct ModuleEntity {
        std::string name;
        std::string description;
        std::string version;
        std::string type;
        std::string language;

        std::vector<ModuleSourceEntity> sources;
        std::vector<std::string> dependencies;

        typedef void DefaultType;
    };
}

BOOST_HANA_ADAPT_STRUCT(bok::ModuleSourceEntity, path, public_);
BOOST_HANA_ADAPT_STRUCT(bok::ModuleEntity, name, description, version, type, language, sources, dependencies);

#endif
