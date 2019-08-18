
#ifndef __BORC_CORE_DEPENDENCYGRAPH_HPP__
#define __BORC_CORE_DEPENDENCYGRAPH_HPP__

#include <map>
#include <vector>
#include <memory>

namespace borc {
    template<typename ValueType>
    class DependencyGraph;

    template<typename ValueType>
    struct DependencyNode {
    public:
        explicit DependencyNode(DependencyGraph<ValueType> *graph) {
            this->graph = graph;
        }

        explicit DependencyNode(DependencyGraph<ValueType> *graph, const ValueType &value) {
            this->graph = graph;

            this->setValue(value);
        }

        explicit DependencyNode(DependencyGraph<ValueType> *graph, const ValueType &value, const DependencyNode *pointee) {
            this->graph = graph;

            this->setValue(value);
            this->setPointee(pointee);
        }

        void setValue(const ValueType value) {
            this->value = value;
        }

        ValueType getValue() const {
            return value;
        }

        void setPointee(const DependencyNode<ValueType>* pointee) {
            this->pointee = pointee;
        }
    
        DependencyNode<ValueType>* getPointee() const {
            return pointee;
        }

        void addPointer(DependencyNode<ValueType>* pointer) {
            pointers.push_back(pointer);
        }

        void removePointer(DependencyNode<ValueType>* pointer) {
            pointers.erase(pointer);
        }

        const std::vector<DependencyNode<ValueType> *>& getPointers() const {
            return pointers;
        }

    private:
        DependencyGraph<ValueType> *graph = nullptr;
        ValueType value;
        DependencyNode *pointee = nullptr;
        std::vector<DependencyNode<ValueType> *> pointers;
    };

    template<typename ValueType>
    class DependencyGraph {
    public:
        DependencyGraph() {
            pointer = new DependencyNode<ValueType>(this);
            nodeStorage.emplace_back(pointer);
        }

        ~DependencyGraph() {}

        DependencyNode<ValueType>* createNode() {
            auto node = new DependencyNode<ValueType>(this);

            nodeStorage.emplace_back(node);

            return node;
        }


        DependencyNode<ValueType>* getPointer() {
            return pointer;
        }


        const DependencyNode<ValueType>* getPointer() const {
            return pointer;
        }

        /*
        DependencyNode<ValueType>* createNode(const ValueType value) {
            auto node = new DependencyNode<ValueType>();

            node->value = value;

            nodeStorage.emplace_back(node);
            valueNodeMap[value] = node;

            return node;
        }
        */

    private:
        DependencyNode<ValueType>* pointer = nullptr;
        std::vector<std::unique_ptr<DependencyNode<ValueType>>> nodeStorage;
        // std::map<ValueType, DependencyNode<ValueType>*> valueNodeMap;
    };
}

#endif
