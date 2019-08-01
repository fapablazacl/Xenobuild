
#ifndef __BORC_UTILITY_DAG_HPP__
#define __BORC_UTILITY_DAG_HPP__

#include <vector>
#include <memory>
#include <boost/optional.hpp>

namespace borc {
    class Command;
    class DagNode;

    class Dag {
    public:
        Dag();

        ~Dag();

        DagNode *createNode() {
            return this->createNode(nullptr);
        }

        DagNode *createNode(Command *command);

        DagNode* getRoot() {
            return root;
        }

        const DagNode* getRoot() const {
            return root;
        }
    
    private:
        DagNode *root;
        std::vector<std::unique_ptr<DagNode>> nodeStorage;
    };
}

#endif
