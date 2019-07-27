
#ifndef __BORC_ENTITY_MODULEENTITY_HPP__
#define __BORC_ENTITY_MODULEENTITY_HPP__

#include <string>
#include <vector>
#include <tuple>

#include <borc/utility/MetaProperty.hpp>

namespace borc {
    struct ModuleSourceEntity {
        std::string path;
        bool public_ = false;

        ModuleSourceEntity() {}

        explicit ModuleSourceEntity(const std::string &path_) : path(path_) {}

        constexpr static auto properties = std::make_tuple (
            property(&ModuleSourceEntity::path, "path"),
            property(&ModuleSourceEntity::public_, "public")
        );

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

        constexpr static auto properties = std::make_tuple (
            property(&ModuleEntity::name, "name"),
            property(&ModuleEntity::description, "description"),
            property(&ModuleEntity::version, "version"),
            property(&ModuleEntity::type, "type"),
            property(&ModuleEntity::language, "language"),
            property(&ModuleEntity::sources, "sources"),
            property(&ModuleEntity::dependencies, "dependencies")
        );

        typedef void DefaultType;
        constexpr static const char* Name = "Module";
    };
}

#endif
