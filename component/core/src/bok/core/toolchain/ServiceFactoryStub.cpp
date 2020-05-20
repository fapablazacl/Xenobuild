
#include "ServiceFactoryStub.hpp"

#include <bok/toolchain/Compiler.hpp>
#include <bok/toolchain/Linker.hpp>
#include <bok/toolchain/SourceChecker.hpp>
#include <bok/toolchain/ModuleChecker.hpp>

namespace bok {
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
