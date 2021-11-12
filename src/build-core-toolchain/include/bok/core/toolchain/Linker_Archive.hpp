
#ifndef __BOK_TOOLCHAIN_ARCHIVELINKER_HPP__
#define __BOK_TOOLCHAIN_ARCHIVELINKER_HPP__

#include <bok/core/toolchain/Linker.hpp>

namespace bok {
    class CommandFactory;

    class Linker_Archive : public Linker {
    public:
        struct Switches {
            std::string buildStaticLibrary;
            std::string moduleOutput;
        };

    public:
        Linker_Archive(CommandFactory *commandFactory, const std::string &commandPath, const Switches &switches);

        virtual ~Linker_Archive();

        virtual LinkOutput link(const boost::filesystem::path &outputPath, const Package *package, const Module *component, const std::vector<boost::filesystem::path> &objectFiles) const override;

    private:
        CommandFactory *commandFactory = nullptr;
        std::string commandPath;
        Switches switches;
    };
}

#endif
