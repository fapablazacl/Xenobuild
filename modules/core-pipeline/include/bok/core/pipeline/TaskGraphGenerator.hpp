
#pragma once

#include <string>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <bok/core/Command.hpp>

namespace bok {
    struct TaskGraphVertexData {
        std::string label;
        boost::filesystem::path filePath;
    };

    struct TaskGraphEdgeData {
        std::string label;
        CommandData command;
    };

    typedef boost::adjacency_list<
        boost::vecS, 
        boost::vecS, 
        boost::directedS, 
        TaskGraphVertexData, TaskGraphEdgeData
    > TaskGraph;

    class Module;
    class Package;
    class Toolchain;
    class TaskGraphGenerator {
    public:
        virtual ~TaskGraphGenerator();

        /**
         * @brief Generate the full task graph required to build the specified Module.
         *
         * The Task Graph consists entirelly of compiler, assmebler and a linker commands.
         */
        virtual TaskGraph generate(Toolchain* toolchain, Module* module) const = 0;
    };
}
