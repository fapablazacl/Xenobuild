
#include <borc/utility/DagVisitor.hpp>

#include <borc/utility/Dag.hpp>
#include <borc/utility/DagNode.hpp>
#include <borc/model/Command.hpp>

namespace borc {    
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
