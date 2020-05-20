
#ifndef __BOK_CORE_DEPENDENCYGRAPH_HPP__
#define __BOK_CORE_DEPENDENCYGRAPH_HPP__

#include <map>
#include <vector>
#include <memory>

namespace bok {
    template<typename ValueType>
    class DependencyGraph;

    template<typename ValueType>
    struct DependencyNode {
    public:
/*
        explicit DependencyNode(DependencyGraph<ValueType> *graph) {
            this->graph = graph;
        }
*/
        explicit DependencyNode(DependencyGraph<ValueType> *graph, const ValueType &value) {
            this->graph = graph;
            this->setValue(value);
        }

        void setValue(const ValueType value) {
            this->value = value;
        }

        ValueType getValue() const {
            return value;
        }

        void addPointee(DependencyNode<ValueType>* pointee) {
            pointees.push_back(pointee);
        }

        void removePointee(DependencyNode<ValueType>* pointee) {
            pointees.erase(pointee);
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

        // what nodes depends on me
        std::vector<DependencyNode<ValueType> *> pointees;

        // what nodes I depends on
        std::vector<DependencyNode<ValueType> *> pointers;
    };

    template<typename ValueType>
    class DependencyGraph {
    public:
        DependencyGraph() {
            pointer = new DependencyNode<ValueType>(this, ValueType{});
            nodeStorage.emplace_back(pointer);
        }

        ~DependencyGraph() {}

        /*
        DependencyNode<ValueType>* createNode() {
            auto node = new DependencyNode<ValueType>(this);

            nodeStorage.emplace_back(node);

            return node;
        }
        */

        DependencyNode<ValueType>* createNode(const ValueType value) {
            auto node = new DependencyNode<ValueType>(this, value);

            nodeStorage.emplace_back(node);
            valueNodeMap.insert({value, node});

            return node;
        }


        DependencyNode<ValueType>* createOrGetNode(const ValueType value) {
            if (auto valueIt=valueNodeMap.find(value); valueIt!=valueNodeMap.end()) {
                return valueIt->second;
            }

            return this->createNode(value);
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
        std::map<ValueType, DependencyNode<ValueType>*> valueNodeMap;
    };
}

#endif
