
#include <borc/utility/Dag.hpp>
#include <borc/utility/DagNode.hpp>

namespace borc {
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
