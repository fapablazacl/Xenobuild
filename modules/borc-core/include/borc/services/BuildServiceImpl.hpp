
#ifndef __BORC_SERVICES_BUILDSERVICEIMPL_HPP__
#define __BORC_SERVICES_BUILDSERVICEIMPL_HPP__

#include "BuildService.hpp"

#include <boost/filesystem/path.hpp>

namespace borc {
    class Toolchain;
    class LoggingService;

    class BuildServiceImpl : public BuildService {
    public:
        BuildServiceImpl(const boost::filesystem::path &basePath, const boost::filesystem::path &outputPath, Toolchain *toolchain, LoggingService *logger);

        virtual ~BuildServiceImpl();

        virtual std::unique_ptr<Dag> createBuildDag(Package *package) override;

    private:
        boost::filesystem::path basePath;
        boost::filesystem::path outputPath;
        Toolchain *toolchain = nullptr;
        LoggingService *logger = nullptr;
    };
}

#endif
