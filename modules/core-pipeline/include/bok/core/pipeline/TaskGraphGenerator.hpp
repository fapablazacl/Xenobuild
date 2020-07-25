
#ifndef __BOK_SERVICES_BUILDSERVICE_HPP__
#define __BOK_SERVICES_BUILDSERVICE_HPP__

#include <string>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace bok {
    struct TaskGraphVertexData {
        std::string label;
        boost::filesystem::path filePath;
    };

    struct TaskGraphEdgeData {
        std::string label;
        std::string command;
    };

    typedef boost::adjacency_list<
        boost::vecS, 
        boost::vecS, 
        boost::directedS, 
        TaskGraphVertexData, TaskGraphEdgeData
    > TaskGraph;

    class Dag;
    class Package;
    class Module;
    class TaskGraphGenerator {
    public:
        virtual ~TaskGraphGenerator();

        [[deprecated]]
        virtual std::unique_ptr<Dag> createBuildDag(Package *package) = 0;

        virtual TaskGraph generate(Module *module) const = 0;

        virtual TaskGraph generate(Package *package) const = 0;
    };
}

#endif
