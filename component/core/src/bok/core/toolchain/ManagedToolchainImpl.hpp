
#ifndef __BOK_TOOLCHAIN_MANAGEDTOOLCHAINIMPL__
#define __BOK_TOOLCHAIN_MANAGEDTOOLCHAINIMPL__

#include "Toolchain.hpp"

#include <vector>
#include <tuple>
#include <memory>

#include <boost/optional/optional_fwd.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/core/noncopyable.hpp>
#include <bok/core/CommandFactory.hpp>

namespace bok {
    struct ToolchainEntity;

    class SourceChecker;
    class ModuleChecker;

    class ManagedToolchainImpl : public Toolchain, private boost::noncopyable {
    public:
        explicit ManagedToolchainImpl(const ToolchainEntity &entity, boost::optional<boost::filesystem::path> installationPath);

        virtual ~ManagedToolchainImpl();

        virtual const Compiler* selectCompiler(const Source *source) const override;

        virtual const Linker* selectLinker(const Component *component) const override;

        virtual Version detectVersion() const override;

    private:
        struct Private;
        Private *m_impl = nullptr;
    };
}

#endif
