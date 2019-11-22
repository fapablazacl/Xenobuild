
#ifndef __BORC_TOOLCHAIN_MANAGEDTOOLCHAINIMPL__
#define __BORC_TOOLCHAIN_MANAGEDTOOLCHAINIMPL__

#include "Toolchain.hpp"

#include <vector>
#include <tuple>
#include <memory>

#include <borc/model/CommandFactory.hpp>

namespace borc {
    struct ToolchainEntity;

    class SourceChecker;
    class ModuleChecker;

    class ManagedToolchainImpl : public Toolchain {
    public:
        explicit ManagedToolchainImpl(const ToolchainEntity &entity);

        virtual ~ManagedToolchainImpl();

        virtual const Compiler* selectCompiler(const Source *source) const override;

        virtual const Linker* selectLinker(const Module *module) const override;

    private:
        std::vector<std::pair<std::unique_ptr<SourceChecker>, std::unique_ptr<Compiler>>> compilers;
        std::vector<std::pair<std::unique_ptr<ModuleChecker>, std::unique_ptr<Linker>>> linkers;

        CommandFactory commandFactory;
    };
}

#endif
