
#ifndef __BOK_TOOLCHAIN_MODULELINKER_HPP__
#define __BOK_TOOLCHAIN_MODULELINKER_HPP__

#include <bok/core/toolchain/Linker.hpp>

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Command;
    class CommandFactory;
    class Package;
    class Module;

    struct LinkerSwitches {
        std::string buildSharedLibrary;
        std::string moduleOutput;
        std::string importLibrary;
        std::string importLibraryPath;

        LinkerSwitches() {}
    };

    struct LinkerConfiguration {
        std::vector<std::string> importLibraries;
        std::vector<std::string> importLibraryPaths;

        LinkerConfiguration() {}
    };

    class Linker_Module : public Linker {
    public:
        explicit Linker_Module(CommandFactory *commandFactory, const std::string &commandPath, const LinkerSwitches &switches, const LinkerConfiguration &configuration);

        virtual ~Linker_Module();

        virtual LinkOutput link(const boost::filesystem::path &outputPath, const Package *package, const Module *component, const std::vector<boost::filesystem::path> &objectFiles) const override;

    private:
        std::vector<std::string> collectLibraries(const Package *package, const Module *component) const;
        std::vector<std::string> collectLibraryPaths(const Package *package, const Module *component, const boost::filesystem::path &outputPath) const;
        
        std::vector<std::string> computeLibrariesOptions(const std::vector<std::string> &libraries) const;
        std::vector<std::string> computeLibraryPathsOptions(const std::vector<std::string> &libraryPaths) const;

    private:
        CommandFactory *commandFactory = nullptr;
        std::string commandPath;
        LinkerSwitches switches;
        LinkerConfiguration configuration;
    };
}

#endif
