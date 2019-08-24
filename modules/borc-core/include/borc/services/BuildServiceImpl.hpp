
#ifndef __BORC_SERVICES_BUILDSERVICEIMPL_HPP__
#define __BORC_SERVICES_BUILDSERVICEIMPL_HPP__

#include "BuildService.hpp"

#include <vector>
#include <boost/filesystem/path.hpp>

namespace borc {
    class Toolchain;
    class LoggingService;
    class Artifact;
    class BuildCache;
    class Command;

    struct CompileOptions;

    class BuildServiceImpl : public BuildService {
    public:
        BuildServiceImpl(const boost::filesystem::path &basePath, const boost::filesystem::path &outputPath, Toolchain *toolchain, BuildCache* buildCache, LoggingService *logger);

        virtual ~BuildServiceImpl();

        virtual std::unique_ptr<Dag> createBuildDag(Package *package) override;

        virtual std::unique_ptr<DependencyBuildGraph> computeDependencyGraph(const Package *package) const override;

    private:
        CompileOptions computeCompileOptions(const Artifact *artifact) const;

        Command* createBuildCacheUpdateCommand(const boost::filesystem::path &filePath);

    private:
        boost::filesystem::path basePath;
        boost::filesystem::path outputPath;
        Toolchain *toolchain = nullptr;
        BuildCache *buildCache = nullptr;
        LoggingService *logger = nullptr;

        std::vector<std::unique_ptr<Command>> commandStorage;
    };
}

#endif
