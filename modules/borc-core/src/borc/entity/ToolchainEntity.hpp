
#ifndef __BORC_ENTITY_TOOLCHAIN_HPP__
#define __BORC_ENTITY_TOOLCHAIN_HPP__

#include <string>
#include <vector>

#include "LanguageEntity.hpp"
#include "ModuleEntity.hpp"

#include <boost/hana.hpp>

namespace borc {
    struct ToolchainEntity {
        struct Switches {
            /* compiler switches */
            std::string debugInformation;
            std::string includePath;
            std::string compile;
            std::string outputFile;
            std::string generateBuildDependencies;

            /* linker switches */
            /*std::string outputFile;*/
            std::string importLibrary;
            std::string libraryPath;
        };

        struct BuildRuleInput {
            std::string fileType;
        };

        struct BuildRuleOutput {
            std::string fileType;
            std::string fileName;
        };

        struct BuildRule {
            BuildRuleInput input;
            std::vector<std::string> flags;
            BuildRuleOutput output;
        };

        struct Tool {
            std::string type;
            std::string command;

            std::vector<BuildRule> buildRules;

            Switches switches;
        };

        std::string name;
        std::vector<Tool> tools;

        typedef void DefaultType;
    };
}

// BOOST_HANA_ADAPT_STRUCT(borc::PackageEntity, name, description, language, modules, packageSearchPaths);

#endif
