
#include <bok/utility/DagNode.hpp>
#include <bok/utility/Dag.hpp>

namespace bok {
    DagNode::DagNode(Dag *dag, Command *command) {
        this->dag = dag;
        this->command = command;
    }

    DagNode* DagNode::createDependency(Command *command) {
        DagNode *dependency = dag->createNode(command);

        dependencies.push_back(dependency);

        return dependency;
    }

    DagNode* DagNode::appendDependency(DagNode* node) {
        dependencies.push_back(node);

        return this;
    }

    void DagNode::setCommand(Command *command) {
        this->command = command;
    }
}
