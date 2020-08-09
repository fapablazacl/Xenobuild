
#pragma once

#include "TaskGraphGenerator.hpp"

#include <vector>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Toolchain;
    class Module;
    
    class Source;
    class Compiler;
    class Linker;

    struct CompileOptions;

    class TaskGraphGenerator_Build : public TaskGraphGenerator {
    public:
        TaskGraph generate(Toolchain* toolchain, Module *module) const override;

    private:
        const Compiler* pickCompiler(const Toolchain* toolchain, const Source* source) const;

        const Linker* pickLinker(const Toolchain* toolchain, const Module* module) const;
    };
}
