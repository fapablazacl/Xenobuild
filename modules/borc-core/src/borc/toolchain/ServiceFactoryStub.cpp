
#include "ServiceFactoryStub.hpp"

#include <borc/toolchain/Compiler.hpp>
#include <borc/toolchain/Linker.hpp>
#include <borc/toolchain/SourceChecker.hpp>
#include <borc/toolchain/ModuleChecker.hpp>

namespace borc {
    ServiceFactoryStub::~ServiceFactoryStub() {}

    const std::vector<std::pair<SourceChecker*, const Compiler*>> ServiceFactoryStub::getCompilers() const {
        std::vector<std::pair<SourceChecker*, const Compiler*>> result;

        for (auto &pair : compilers) {
            result.push_back( {pair.first.get(), pair.second.get()} );
        }

        return result;
    }

    const std::vector<std::pair<ModuleChecker*, const Linker*>> ServiceFactoryStub::getLinkers() const {
        std::vector<std::pair<ModuleChecker*, const Linker*>> result;

        for (auto &pair : linkers) {
            result.push_back( {pair.first.get(), pair.second.get()} );
        }

        return result;
    }
}
