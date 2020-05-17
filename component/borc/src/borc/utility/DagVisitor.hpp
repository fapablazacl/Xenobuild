
#ifndef __BORC_UTILITY_DAGVISITOR_HPP__
#define __BORC_UTILITY_DAGVISITOR_HPP__

namespace bok {
    class Dag;
    class DagNode;
    class DagVisitor {
    public:
        void visit(Dag *dag);

    private:
        void visitNode(DagNode *dagNode);
    };
}

#endif
