
#include "ToolchainImpl.hpp"

#include <algorithm>
#include <borc/model/Source.hpp>
#include <borc/toolchain/SourceChecker.hpp>
#include <borc/toolchain/ArtifactChecker.hpp>

namespace borc {
    ToolchainImpl::ToolchainImpl(const std::vector<std::pair<SourceChecker*, const Compiler*>> &compilers, const std::vector<std::pair<ArtifactChecker*, const Linker*>> &linkers) {
        this->compilers = compilers;
        this->linkers = linkers;
    }

    ToolchainImpl::~ToolchainImpl() {}

    const Compiler* ToolchainImpl::selectCompiler(const Source *source) const {
        for (auto &pair : compilers) {
            if (pair.first->check(source->getFilePath())) {
                return pair.second;
            }
        }

        return nullptr;
    }
    
    const Linker* ToolchainImpl::selectLinker(const Artifact *artifact) const {
        for (auto &pair : linkers) {
            if (pair.first->check(artifact)) {
                return pair.second;
            }
        }

        return nullptr;
    }
}
