
#ifndef __BORC_SERVICES_BUILDSERVICEIMPL_HPP__
#define __BORC_SERVICES_BUILDSERVICEIMPL_HPP__

#include "BuildService.hpp"

#include <boost/filesystem/path.hpp>

namespace borc {
    class Toolchain;
    class LoggingService;
    class Artifact;
    class BuildCache;

    struct CompileOptions;

    class BuildServiceImpl : public BuildService {
    public:
        BuildServiceImpl(const boost::filesystem::path &basePath, const boost::filesystem::path &outputPath, Toolchain *toolchain, BuildCache* buildCache, LoggingService *logger);

        virtual ~BuildServiceImpl();

        virtual std::unique_ptr<Dag> createBuildDag(Package *package) override;

    private:
        CompileOptions computeCompileOptions(const Artifact *artifact) const;

    private:
        boost::filesystem::path basePath;
        boost::filesystem::path outputPath;
        Toolchain *toolchain = nullptr;
        BuildCache *buildCache = nullptr;
        LoggingService *logger = nullptr;
    };
}

#endif
