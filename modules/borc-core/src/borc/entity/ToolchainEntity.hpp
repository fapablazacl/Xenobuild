
#ifndef __BORC_ENTITY_TOOLCHAINENTITY_HPP__
#define __BORC_ENTITY_TOOLCHAINENTITY_HPP__

#include <string>
#include <vector>

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

            typedef void DefaultType;
        };

        struct BuildRuleInput {
            std::string fileType;

            typedef void DefaultType;
        };

        struct BuildRuleOutput {
            std::string fileType;
            std::string fileName;

            typedef void DefaultType;
        };

        struct BuildRule {
            BuildRuleInput input;
            std::vector<std::string> flags;
            BuildRuleOutput output;

            typedef void DefaultType;
        };

        struct Tool {
            std::string type;
            std::string command;

            std::vector<BuildRule> buildRules;

            Switches switches;

            typedef void DefaultType;
        };

        std::string name;
        std::vector<Tool> tools;

        typedef void DefaultType;
    };
}

#endif
