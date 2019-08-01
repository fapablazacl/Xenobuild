
#ifndef __BORC_UTILITY_DAGNODE_HPP__
#define __BORC_UTILITY_DAGNODE_HPP__

#include <vector>

namespace borc {
    class Command;

    struct DagNode {
        Command *command = nullptr;
        std::vector<DagNode*> previous;
    };
}

#endif
