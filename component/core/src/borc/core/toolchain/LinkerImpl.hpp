
#ifndef __BOK_TOOLCHAIN_LINKERIMPL_HPP__
#define __BOK_TOOLCHAIN_LINKERIMPL_HPP__

#include "Linker.hpp"

#include <boost/optional/optional_fwd.hpp>
#include <borc/model/Module.hpp>

namespace bok {
	class CommandFactory;

    class LinkerImpl : public Linker {
    public:
        struct Switches {
            std::string outputFile;
            std::string importLibrary;
            std::string libraryPath;
        };

        struct BuildRule {
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
        

    public:
        LinkerImpl(CommandFactory *commandFactory, const boost::filesystem::path &linkerPath, const std::string &commandName, const LinkerImpl::Switches &switches, const std::vector<LinkerImpl::BuildRule> &buildRules);

        virtual ~LinkerImpl();

        virtual bool isModuleLinkable(const Module *module) const override;

        virtual LinkOutput link(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles) const override;

    private:
        struct Private;
        Private *m_impl = nullptr;
    };
}

#endif
