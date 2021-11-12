
#ifndef __BOK_ENTITY_MODULEENTITY_HPP__
#define __BOK_ENTITY_MODULEENTITY_HPP__

#include <string>
#include <vector>

#include <boost/hana.hpp>

namespace bok {
    struct ComponentSourceEntity {
        std::string path;
        bool public_ = false;

        ComponentSourceEntity() {}

        explicit ComponentSourceEntity(const std::string &path_) : path(path_) {}

        explicit ComponentSourceEntity(const std::string &path_, const bool public__) : path(path_), public_(public__) {}

        typedef std::string DefaultType;
    };

    struct ComponentEntity {
        std::string name;
        std::string description;
        std::string version;
        std::string type;
        std::string language;

        std::vector<ComponentSourceEntity> sources;
        std::vector<std::string> dependencies;

        typedef void DefaultType;
    };
}

BOOST_HANA_ADAPT_STRUCT(bok::ComponentSourceEntity, path, public_);
BOOST_HANA_ADAPT_STRUCT(bok::ComponentEntity, name, description, version, type, language, sources, dependencies);

#endif
