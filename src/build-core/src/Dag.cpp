
#include <bok/core/Dag.hpp>
#include <bok/core/DagNode.hpp>

namespace bok {
    Dag::Dag() {
        root = this->createNode();
    }

    Dag::~Dag() {}

    DagNode *Dag::createNode(Command *command) {
        auto node = new DagNode{this, command};

        nodeStorage.emplace_back(node);

        return node;
    }
}
