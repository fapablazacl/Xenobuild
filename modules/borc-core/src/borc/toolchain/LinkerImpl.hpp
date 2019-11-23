
#ifndef __BORC_TOOLCHAIN_LINKERIMPL_HPP__
#define __BORC_TOOLCHAIN_LINKERIMPL_HPP__

#include "Linker.hpp"

#include <boost/optional/optional_fwd.hpp>
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


	class CommandFactory;

    class LinkerImpl : public Linker {
    public:
        LinkerImpl(CommandFactory *commandFactory, const boost::filesystem::path &linkerPath, const std::string &commandName, const LinkerImplSwitches &switches, const std::vector<LinkerImplBuildRule> &buildRules);

        virtual ~LinkerImpl();

        virtual LinkOutput link(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles) const override;


	private:
		boost::optional<LinkerImplBuildRule> matchBuildRule(const Module *module) const;

		Command* buildLinkCommand(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles, const LinkerImplBuildRule &buildRule) const;

		boost::filesystem::path buildOuputPath(const boost::filesystem::path &outputPath, const Package *package, const Module *module) const;

		std::vector<std::string> buildModuleDependencyOptions(const boost::filesystem::path &outputPath, const Package *package, const Module *dependency) const;

		boost::filesystem::path buildCommandPath() const;

    private:
		CommandFactory *commandFactory = nullptr;
        boost::filesystem::path linkerPath; 
        std::string commandName; 
        LinkerImplSwitches switches;
        std::vector<LinkerImplBuildRule> buildRules;
    };
}

#endif
