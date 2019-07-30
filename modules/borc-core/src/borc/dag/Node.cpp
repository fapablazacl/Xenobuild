
#include "Node.hpp"
#include <algorithm>

namespace borc {
    Node::Node(const std::string &filePath) {
        this->filePath = filePath;
    }

    Node::~Node() {}

    void Node::addDependency(Node *dependency) {
        dependencies.push_back(dependency);
    }

    void Node::removeDependency(Node *child) {
        auto location = std::find_if(dependencies.begin(), dependencies.end(), [child](auto node) {
            return node == child;
        });

        if (location != dependencies.end()) {
            dependencies.erase(location);
        }
    }

    std::vector<Node*> Node::getDependencies() const {
        return dependencies;
    }

    std::string Node::getFilePath() const {
        return filePath;
    }
}
