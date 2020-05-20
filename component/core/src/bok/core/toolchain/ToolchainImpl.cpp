
#include "ToolchainImpl.hpp"

#include <algorithm>
#include <bok/model/Source.hpp>
#include <bok/toolchain/SourceChecker.hpp>
#include <bok/toolchain/ModuleChecker.hpp>

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
    
    const Linker* ToolchainImpl::selectLinker(const Module *module) const {
        for (auto &pair : linkers) {
            if (pair.first->check(module)) {
                return pair.second;
            }
        }

        return nullptr;
    }
}
