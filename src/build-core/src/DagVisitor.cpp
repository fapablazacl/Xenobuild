
#include <bok/core/DagVisitor.hpp>
#include <bok/core/Dag.hpp>
#include <bok/core/DagNode.hpp>
#include <bok/core/Command.hpp>

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
