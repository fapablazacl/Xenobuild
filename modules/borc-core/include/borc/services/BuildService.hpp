
#ifndef __BORC_SERVICES_BUILDSERVICE_HPP__
#define __BORC_SERVICES_BUILDSERVICE_HPP__

#include <string>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace borc {
    struct DependencyGraphVertexData {
        boost::filesystem::path filePath;
    };

    struct DependencyGraphEdgeData {
        std::string command;
    };

    typedef boost::adjacency_list<
        boost::vecS, boost::vecS, boost::directedS, 
        DependencyGraphVertexData, DependencyGraphEdgeData
    > DependencyGraph;

    class Dag;
    class Package;
    class Artifact;
    class BuildService {
    public:
        virtual ~BuildService();

        virtual std::unique_ptr<Dag> createBuildDag(Package *package) = 0;

        virtual DependencyGraph computeDependencyGraph(Artifact *artifact) const = 0;
    };
}

#endif
