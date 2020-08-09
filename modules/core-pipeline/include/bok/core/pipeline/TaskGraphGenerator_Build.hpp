
#ifndef __BOK_SERVICES_BUILDSERVICEIMPL_HPP__
#define __BOK_SERVICES_BUILDSERVICEIMPL_HPP__

#include "TaskGraphGenerator.hpp"

#include <vector>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Toolchain;
    class Logger;
    class Module;
    class BuildCache;
    class Command;

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

#endif
