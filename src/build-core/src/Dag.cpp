
#include <xb/core/Dag.h>
#include <xb/core/DagNode.h>

namespace Xenobuild {
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
