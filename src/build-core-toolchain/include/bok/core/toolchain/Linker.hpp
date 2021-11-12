
#pragma once

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <bok/core/Command.hpp>

namespace bok {
    enum class LinkerModuleType {
        GuiApplication,
        CliApplication,
        StaticLibrary,
        DynamicLibrary
    };

    struct LinkInput {
        std::string moduleName;
        boost::filesystem::path outputPath;
        std::vector<boost::filesystem::path> objectFiles;
        LinkerModuleType moduleType;
        std::vector<boost::filesystem::path> libraryPaths;
        std::vector<boost::filesystem::path> libraries;
    };

    struct LinkOutput {
        boost::filesystem::path outputModuleRelativePath;

        CommandData linkCommand;

        [[deprecated]]
        Command *command = nullptr;
    };

    struct LinkerOptions {};
    class Command;
    class Package;
    class Module;

    class Linker {
    public:
        virtual ~Linker();

        virtual LinkOutput generateLinkOutput(const LinkInput &input) const = 0;

        [[deprecated]]
        virtual bool isModuleLinkable(const Module *component) const {
            return false;
        }

        [[deprecated]]
        virtual LinkOutput link(const boost::filesystem::path& outputPath, const Package* package, const Module* component, const std::vector<boost::filesystem::path>& objectFiles) const {
            return {};
        }
    };
}
