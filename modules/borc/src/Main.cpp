
#include <iostream>
#include <stdexcept>
#include <vector>

#include "borc/cli/Controller.hpp"
#include "borc/cli/ControllerFactory.hpp"

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            throw std::runtime_error("No subcommand supplied.");
        }

        std::vector<char*> args = {argv + 1, argv + argc};

        borc::ControllerFactory controllerFactory;
        auto controller = controllerFactory.create(args[0]);

        controller->perform(args.size(), args.data());
    
        return 0;
    } catch (const std::exception &exp) {
        std::cout << "ERROR: " << exp.what() << std::endl;

        return 1;
    }

    return 0;
}


/*
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

struct VertexProperties {
    std::string name;
};

boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS, VertexProperties> createEmptyDirectedGraph() noexcept {
    return {};
}

int main(int argc, char **argv) {
    // Un vertice se maneja con un Handle
    auto directedGraph = createEmptyDirectedGraph();
    auto v1 = boost::add_vertex(directedGraph);
    auto v2 = boost::add_vertex(directedGraph);
    auto v3 = boost::add_vertex(directedGraph);
    auto v4 = boost::add_vertex(directedGraph);
    auto v5 = boost::add_vertex(directedGraph);
    auto v6 = boost::add_vertex(directedGraph);

    directedGraph[v1].name = "Hola";

    auto edgeResult1 = boost::add_edge(v1, v2, directedGraph);
    auto edgeResult2 = boost::add_edge(v2, v3, directedGraph);
    auto edgeResult3 = boost::add_edge(v1, v3, directedGraph);
    auto edgeResult4 = boost::add_edge(v3, v4, directedGraph);
    auto edgeResult5 = boost::add_edge(v4, v5, directedGraph);
    auto edgeResult6 = boost::add_edge(v5, v6, directedGraph);
    auto edgeResult7 = boost::add_edge(v3, v6, directedGraph);

    boost::write_graphviz(std::cout, directedGraph);

    return 0;
}
*/
