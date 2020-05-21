
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

    class BuildTaskGraphGenerator : public TaskGraphGenerator {
    public:
        BuildTaskGraphGenerator(const boost::filesystem::path &basePath, const boost::filesystem::path &outputPath, Toolchain *toolchain, BuildCache* buildCache, LoggingService *logger);

        virtual ~BuildTaskGraphGenerator();

        std::unique_ptr<Dag> createBuildDag(Package *package) override;

        TaskGraph generate(Component *component) const override;

        TaskGraph generate(Package *package) const override;

    private:
        struct Private;
        Private* m_impl = nullptr;
    };
}

#endif
