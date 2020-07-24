
#include "ToolchainImpl.hpp"

#include <algorithm>
#include <bok/core/package/Source.hpp>
#include <bok/core/toolchain/SourceChecker.hpp>
#include <bok/core/toolchain/ModuleChecker.hpp>

namespace bok {
    ToolchainImpl::ToolchainImpl(const std::vector<std::pair<SourceChecker*, const Compiler*>> &compilers, const std::vector<std::pair<ModuleChecker*, const Linker*>> &linkers) {
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
    
    const Linker* ToolchainImpl::selectLinker(const Component *component) const {
        for (auto &pair : linkers) {
            if (pair.first->check(component)) {
                return pair.second;
            }
        }

        return nullptr;
    }
}
