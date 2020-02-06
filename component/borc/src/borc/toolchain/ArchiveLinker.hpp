
#ifndef __BORC_TOOLCHAIN_ARCHIVELINKER_HPP__
#define __BORC_TOOLCHAIN_ARCHIVELINKER_HPP__

#include <borc/toolchain/Linker.hpp>

namespace borc {
    class CommandFactory;

    class ArchiveLinker : public Linker {
    public:
        struct Switches {
            std::string buildStaticLibrary;
            std::string moduleOutput;
        };

    public:
        ArchiveLinker(CommandFactory *commandFactory, const std::string &commandPath, const Switches &switches);

        virtual ~ArchiveLinker();

        virtual LinkOutput link(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles) const override;

    private:
        CommandFactory *commandFactory = nullptr;
        std::string commandPath;
        Switches switches;
    };
}

#endif