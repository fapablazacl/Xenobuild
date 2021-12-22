
#ifndef __BOK_UTILITY_DAG_HPP__
#define __BOK_UTILITY_DAG_HPP__

#include <vector>
#include <memory>
#include <boost/optional.hpp>

namespace Xenobuild {
    class Command;
    class DagNode;

    class Dag {
    public:
        Dag();

        ~Dag();

        DagNode* getRoot() {
            return root;
        }

        const DagNode* getRoot() const {
            return root;
        }
    
        DagNode *createNode() {
            return this->createNode(nullptr);
        }

        DagNode *createNode(Command *command);

    private:
        DagNode *root;
        std::vector<std::unique_ptr<DagNode>> nodeStorage;
    };
}

#endif
