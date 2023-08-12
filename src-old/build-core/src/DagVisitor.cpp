
#include <Xenobuild/core/DagVisitor.h>
#include <Xenobuild/core/Dag.h>
#include <Xenobuild/core/DagNode.h>
#include <Xenobuild/core/Command.h>

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
