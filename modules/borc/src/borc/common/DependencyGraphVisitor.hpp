
#ifndef __BORC_COMMON_DEPENDENCYGRAPHVISITOR_HPP__
#define __BORC_COMMON_DEPENDENCYGRAPHVISITOR_HPP__

#include <borc/services/BuildService.hpp>
#include <iostream>

namespace borc {
    class DependencyGraphVisitor {
    public:
        virtual ~DependencyGraphVisitor() {}

        virtual void visit(const DependencyBuildGraph *graph) = 0;
    };

    class DependencyGraphVisitorImpl : public DependencyGraphVisitor {
    public:
        virtual void visit(const DependencyBuildGraph *graph) override {
            this->visit(graph->getPointer());
        }

    private:
        void visit(const DependencyBuildNode *node, const int indent = 0) {
            for (int i=0; i<indent; i++) {
                std::cout << " ";
            }

            std::cout << node->getValue() << std::endl;

            for (const DependencyBuildNode *pointer : node->getPointers()) {
                this->visit(pointer, indent + 2);
            }
        }
    };

    class BuildDependencyGraphVisitor : public DependencyGraphVisitor {
    public:
        virtual void visit(const DependencyBuildGraph *graph) override {
            this->visit(graph->getPointer());
        }

    private:
        void visit(const DependencyBuildNode *node) {
            for (const DependencyBuildNode *pointer : node->getPointers()) {
                this->visit(pointer);
            }
        }
    };
}

#endif
