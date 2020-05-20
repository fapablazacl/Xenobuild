
#ifndef __BOK_CORE_TOOLCHAINIMPL_HPP__
#define __BOK_CORE_TOOLCHAINIMPL_HPP__

#include <set>
#include <boost/filesystem.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/model/Module.hpp>

namespace bok {
    class Compiler;
    class Linker;
    class Source;
    class Module;
    class SourceChecker;
    class ModuleChecker;

    class ToolchainImpl : public Toolchain {
    public:
        explicit ToolchainImpl(const std::vector<std::pair<SourceChecker*, const Compiler*>> &compilers, const std::vector<std::pair<ModuleChecker*, const Linker*>> &linkers);

        virtual ~ToolchainImpl();

        virtual const Compiler* selectCompiler(const Source *source) const override;

        virtual const Linker* selectLinker(const Module *module) const override;

    private:
        std::vector<std::pair<SourceChecker*, const Compiler*>> compilers;
        std::vector<std::pair<ModuleChecker*, const Linker*>> linkers;
    };
}

#endif
