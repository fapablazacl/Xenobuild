
#include "NodeRepository.hpp"
#include "Node.hpp"

namespace borc {
    NodeRepository::NodeRepository() {}

    NodeRepository::~NodeRepository() {}

    Node* NodeRepository::getNode(const std::string &filePath) const {
        auto it = nodeMap.find(filePath);

        if (it != nodeMap.end()) {
            return it->second.get();
        } else {
            auto nodePtr = std::make_unique<Node>(filePath);
            auto node = nodePtr.get();

            nodeMap[filePath] = std::move(nodePtr);

            return node;
        }
    }
}
