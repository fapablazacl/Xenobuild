
#include "ToolchainImpl.hpp"

#include <algorithm>
#include <borc/model/Source.hpp>

namespace borc {
    SourceType::SourceType(const std::initializer_list<std::string> &wildcards) {
        this->wildcards = wildcards;
    }

    bool SourceType::match(const boost::filesystem::path &filePath) const {
        const std::string ext = "*" + filePath.extension().string();

        auto it = std::find(wildcards.begin(), wildcards.end(), ext);

        return it != wildcards.end();
    }
}

namespace borc {
    ToolchainImpl::ToolchainImpl(const std::vector<std::pair<SourceType, const Compiler*>> &compilers, const std::vector<std::pair<ArtifactChecker*, const Linker*>> &linkers) {
        this->compilers = compilers;
        this->linkers = linkers;
    }

    ToolchainImpl::~ToolchainImpl() {}

    const Compiler* ToolchainImpl::selectCompiler(const Source *source) const {
        for (auto &pair : compilers) {
            if (pair.first.match(source->getFilePath())) {
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
