
set (target utility)

set (sources 
    ./src/bok/utility/DagNode.hpp
    ./src/bok/utility/Dag.hpp
    ./src/bok/utility/DagVisitor.hpp
    ./src/bok/utility/Dag.cpp
    ./src/bok/utility/DependencyGraph.hpp
    ./src/bok/utility/DagVisitor.cpp
    ./src/bok/utility/DagNode.cpp
)

add_library(${target} ${sources})
