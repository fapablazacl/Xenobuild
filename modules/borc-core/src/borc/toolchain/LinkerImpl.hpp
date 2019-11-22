
#ifndef __BORC_TOOLCHAIN_LINKERIMPL_HPP__
#define __BORC_TOOLCHAIN_LINKERIMPL_HPP__

#include "Linker.hpp"

#include <borc/model/Module.hpp>

namespace borc {
    struct LinkerImplSwitches {
        std::string outputFile;
        std::string importLibrary;
        std::string libraryPath;
    };


    struct LinkerImplBuildRule {
        struct Input {
            std::vector<Module::Type> moduleTypes;
            std::string fileType;
        };

        struct Output {
            std::string fileType;
            std::string fileName;
        };

        Input input;
        std::vector<std::string> flags;
        Output output;
    };


    class LinkerImpl : public Linker {
    public:
        LinkerImpl(const boost::filesystem::path &linkerPath, const std::string &command, const LinkerImplSwitches &switches, const std::vector<LinkerImplBuildRule> &buildRules);

        virtual ~LinkerImpl();

        virtual LinkOutput link(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles) const override;

    private:
        boost::filesystem::path linkerPath; 
        std::string command; 
        LinkerImplSwitches switches;
        std::vector<LinkerImplBuildRule> buildRules;
    };
}

#endif
