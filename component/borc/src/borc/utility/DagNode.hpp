
#ifndef __BORC_UTILITY_DAGNODE_HPP__
#define __BORC_UTILITY_DAGNODE_HPP__

#include <vector>

namespace bok {
    class Command;

    class Dag;
    class DagNode {
    public:
        DagNode(Dag *dag, Command *command);

        Command* getCommand() const {
            return command;
        }

        void setCommand(Command *command);

        const std::vector<DagNode*>& getDependencies() const {
            return dependencies;
        }

        DagNode* createDependency(Command *command);

        DagNode* createDependency() {
            return this->createDependency(nullptr);
        }

        DagNode* appendDependency(DagNode* node);

    private:
        Dag *dag = nullptr;
        Command *command = nullptr;
        std::vector<DagNode*> dependencies;
    };
}

#endif
