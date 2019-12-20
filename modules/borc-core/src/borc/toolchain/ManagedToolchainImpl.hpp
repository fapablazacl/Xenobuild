
#ifndef __BORC_TOOLCHAIN_MANAGEDTOOLCHAINIMPL__
#define __BORC_TOOLCHAIN_MANAGEDTOOLCHAINIMPL__

#include "Toolchain.hpp"

#include <vector>
#include <tuple>
#include <memory>

#include <boost/core/noncopyable.hpp>
#include <borc/model/CommandFactory.hpp>


namespace borc {
    struct ToolchainEntity;

    class SourceChecker;
    class ModuleChecker;

    class ManagedToolchainImpl : public Toolchain, private boost::noncopyable {
    public:
        explicit ManagedToolchainImpl(const ToolchainEntity &entity);

        virtual ~ManagedToolchainImpl();

        virtual const Compiler* selectCompiler(const Source *source) const override;

        virtual const Linker* selectLinker(const Module *module) const override;

        virtual Version detectVersion() const override;

    private:
        struct Private;
        Private *m_impl = nullptr;
    };
}

#endif
