
#ifndef __BORC_ENTITY_PACKAGE_HPP__
#define __BORC_ENTITY_PACKAGE_HPP__

#include <string>
#include <vector>

#include "LanguageEntity.hpp"
#include "ModuleEntity.hpp"

#include <boost/hana.hpp>

namespace borc {
    struct PackageEntity {
        struct Required {
            std::string name;
            std::string type;
            std::string desc;

            typedef void DefaultType;
        };

        struct Module {
            bool common;
            bool prebuilt;
            std::string includePath;
            std::string libraryPath;

            std::string name;
            std::string library;

            typedef void DefaultType;
        };

        std::string name;
        std::string description;
        std::vector<Required> required;
        LanguageEntity language;
        std::vector<std::string> modulePaths;
        std::vector<std::string> packageSearchPaths;

        std::vector<Module> modules;

        typedef void DefaultType;
    };
}

BOOST_HANA_ADAPT_STRUCT(borc::PackageEntity::Required, name, type, desc);
BOOST_HANA_ADAPT_STRUCT(borc::PackageEntity::Module, common, prebuilt, includePath, libraryPath, name, library);
BOOST_HANA_ADAPT_STRUCT(borc::PackageEntity, name, description, required, language, modulePaths, packageSearchPaths, modules);

#endif
