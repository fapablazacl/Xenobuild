
#ifndef __BOK_SERVICES_BUILDSERVICEIMPL_HPP__
#define __BOK_SERVICES_BUILDSERVICEIMPL_HPP__

#include "BuildService.hpp"

#include <vector>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Toolchain;
    class LoggingService;
    class Component;
    class BuildCache;
    class Command;

    struct CompileOptions;

    class BuildServiceImpl : public BuildService {
    public:
        BuildServiceImpl(const boost::filesystem::path &basePath, const boost::filesystem::path &outputPath, Toolchain *toolchain, BuildCache* buildCache, LoggingService *logger);

        virtual ~BuildServiceImpl();

        virtual std::unique_ptr<Dag> createBuildDag(Package *package) override;

        virtual DependencyGraph computeDependencyGraph(Component *component) const override;

    private:
        struct Private;
        Private* m_impl = nullptr;
    };
}

#endif
