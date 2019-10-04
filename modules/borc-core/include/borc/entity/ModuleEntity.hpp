
#ifndef __BORC_ENTITY_MODULEENTITY_HPP__
#define __BORC_ENTITY_MODULEENTITY_HPP__

#include <string>
#include <vector>

#include <boost/hana.hpp>

namespace borc {
    struct ModuleSourceEntity {
        std::string path;
        bool public_ = false;
    };

    struct ModuleEntity {
        std::string name;
        std::string description;
        std::string version;
        std::string type;
        std::string language;

        std::vector<ModuleSourceEntity> sources;
        std::vector<std::string> dependencies;
    };
}

BOOST_HANA_ADAPT_STRUCT(borc::ModuleSourceEntity, path, public_);
BOOST_HANA_ADAPT_STRUCT(borc::ModuleEntity, name, description, version, type, language, sources, dependencies);

#endif
