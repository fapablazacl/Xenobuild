
#include <borc/dag/BuildGraphGenerator.hpp>
#include <borc/dag/NodeRepository.hpp>
#include <borc/dag/Node.hpp>

#include <string>
/*
#include <borc/core/Compiler.hpp>
#include <borc/core/Linker.hpp>
#include <borc/core/Toolchain.hpp>
#include <borc/core/Module.hpp>
#include <borc/core/Source.hpp>
*/

namespace borc {
    BuildGraphGenerator::BuildGraphGenerator(NodeRepository *nodeRepository, const Toolchain *toolchain) {
        this->nodeRepository = nodeRepository;
        this->toolchain = toolchain;
    }

    Node* BuildGraphGenerator::generateGraph(const Artifact *artifact) const {
        /*
        const std::string moduleFilePath = artifact->getOutputFilePath().string();
        Node *moduleNode = nodeRepository->getNode(moduleFilePath);

        for (const borc::Source *source: artifact->getSources()) {
            Node *objectNode = this->generateGraph(source);

            if (!objectNode) {
                continue;
            }

            moduleNode->addDependency(objectNode);
        }

        return moduleNode;
        */

        return nullptr;
    }

    Node* BuildGraphGenerator::generateGraph(const Source *source) const {
        /*
        const auto compiler = this->toolchain->selectCompiler(source);

        if (!compiler) {
            return nullptr;
        }

        const auto objectFilePath = compiler->getObjectFilePath(source);
        const auto objectNode = nodeRepository->getNode(objectFilePath.string());

        const auto sourceFilePath = source->getFilePath();
        const auto sourceNode = nodeRepository->getNode(sourceFilePath.string());

        objectNode->addDependency(sourceNode);

        // TODO: Compute header dependency information

        return objectNode;
        */

       return nullptr;
    }
}
