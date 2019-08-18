
#ifndef __BORC_SERVICES_BUILDSERVICE_HPP__
#define __BORC_SERVICES_BUILDSERVICE_HPP__

#include <string>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <borc/core/DependencyGraph.hpp>

namespace borc {
    typedef DependencyNode<boost::filesystem::path> DependencyBuildNode;
    typedef DependencyGraph<boost::filesystem::path> DependencyBuildGraph;

    class Dag;
    class Package;
    class BuildService {
    public:
        virtual ~BuildService();

        virtual std::unique_ptr<Dag> createBuildDag(Package *package) = 0;

        virtual std::unique_ptr<DependencyBuildGraph> computeDependencyGraph(const Package *package) const = 0;
    };
}

#endif
