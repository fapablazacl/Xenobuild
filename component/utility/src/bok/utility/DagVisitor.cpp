
#include <bok/utility/DagVisitor.hpp>

#include <bok/utility/Dag.hpp>
#include <bok/utility/DagNode.hpp>
#include <bok/model/Command.hpp>

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
