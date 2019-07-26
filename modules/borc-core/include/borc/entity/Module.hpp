
#ifndef __BORC_ENTITY_MODULE_HPP__
#define __BORC_ENTITY_MODULE_HPP__

#include <string>
#include <vector>
#include <tuple>

#include <borc/utility/MetaProperty.hpp>

namespace borc {
    struct ModuleSource {
        std::string path;
        bool public_ = false;

        ModuleSource() {}

        explicit ModuleSource(const std::string &path_) : path(path_) {}

        constexpr static auto properties = std::make_tuple (
            property(&ModuleSource::path, "path"),
            property(&ModuleSource::public_, "public")
        );

        typedef std::string DefaultType;
    };

    struct Module {
        std::string name;
        std::string description;
        std::string version;
        std::string type;
        std::string language;

        std::vector<ModuleSource> sources;
        std::vector<std::string> dependencies;

        constexpr static auto properties = std::make_tuple (
            property(&Module::name, "name"),
            property(&Module::description, "description"),
            property(&Module::version, "version"),
            property(&Module::type, "type"),
            property(&Module::language, "language"),
            property(&Module::sources, "sources"),
            property(&Module::dependencies, "dependencies")
        );

        typedef void DefaultType;
        constexpr static const char* Name = "Module";
    };
}

#endif
