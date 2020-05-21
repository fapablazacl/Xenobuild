
#include <bok/core/common/DagVisitor.hpp>
#include <bok/core/common/Dag.hpp>
#include <bok/core/common/DagNode.hpp>
#include <bok/core/common/Command.hpp>

namespace bok {    
    void DagVisitor::visit(Dag *dag) {
        this->visitNode(dag->getRoot());
    }


    void DagVisitor::visitNode(DagNode *dagNode) {
        for (DagNode *dependency : dagNode->getDependencies()) {
            this->visitNode(dependency);
        }

        if (dagNode->getCommand()) {
            dagNode->getCommand()->execute();
        }
    }
}
