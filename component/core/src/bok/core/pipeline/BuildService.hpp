
#ifndef __BOK_SERVICES_BUILDSERVICE_HPP__
#define __BOK_SERVICES_BUILDSERVICE_HPP__

#include <string>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace bok {
    struct DependencyGraphVertexData {
        std::string label;
        boost::filesystem::path filePath;
    };

    struct DependencyGraphEdgeData {
        std::string label;
        std::string command;
    };

    typedef boost::adjacency_list<
        boost::vecS, boost::vecS, boost::directedS, 
        DependencyGraphVertexData, DependencyGraphEdgeData
    > DependencyGraph;

    class Dag;
    class Package;
    class Component;
    class DependencyGraphGenerator {
    public:
        virtual ~DependencyGraphGenerator();

        [[deprecated]]
        virtual std::unique_ptr<Dag> createBuildDag(Package *package) = 0;

        virtual DependencyGraph generate(Component *component) const = 0;

        virtual DependencyGraph generate(Package *package) const = 0;
    };
}

#endif
