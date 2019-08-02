
#include <borc/utility/DagVisitor.hpp>

#include <borc/utility/Dag.hpp>
#include <borc/utility/DagNode.hpp>
#include <borc/model/command/Command.hpp>

namespace borc {    
    void DagVisitor::visit(Dag *dag) {
        this->visitNode(dag->getRoot());
    }


    void DagVisitor::visitNode(DagNode *dagNode) {
        for (DagNode *dependency : dagNode->previous) {
            this->visitNode(dependency);
        }

        if (dagNode->command) {
            dagNode->command->execute();
        }
    }
}
