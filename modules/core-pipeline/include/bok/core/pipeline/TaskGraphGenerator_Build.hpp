
#pragma once

#include "TaskGraphGenerator.hpp"

#include <vector>
#include <boost/filesystem/path.hpp>
#include <bok/core/toolchain/Toolchain.hpp>
#include <bok/utility/WildcardClassifier.hpp>

namespace bok {
    class Toolchain;
    class Module;
    
    class Source;
    class Compiler;
    class Linker;

    struct CompileOptions;

    class TaskGraphGenerator_Build : public TaskGraphGenerator {
    public:
        TaskGraphGenerator_Build(const WildcardClassifier<CompilerType> &sourceClassifier);

        TaskGraph generate(Toolchain* toolchain, Module *module) const override;

    private:
        const Compiler* pickCompiler(const Toolchain* toolchain, const Source* source) const;

        const Linker* pickLinker(const Toolchain* toolchain, const Module* module) const;

    private:
        const WildcardClassifier<CompilerType> &sourceClassifier;
    };
}
