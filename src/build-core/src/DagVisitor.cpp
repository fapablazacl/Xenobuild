
#include <xb/core/DagVisitor.h>
#include <xb/core/Dag.h>
#include <xb/core/DagNode.h>
#include <xb/core/Command.h>

namespace Xenobuild {    
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
