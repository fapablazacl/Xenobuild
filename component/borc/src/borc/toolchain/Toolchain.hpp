
#ifndef __BORC_CORE_TOOLCHAIN_HPP__
#define __BORC_CORE_TOOLCHAIN_HPP__

namespace bok {
    class Compiler;
    class Linker;
    class Source;
    class Module;

    struct Version;

    class Toolchain {
    public:
        virtual ~Toolchain();

        virtual const Compiler* selectCompiler(const Source *source) const = 0;

        virtual const Linker* selectLinker(const Module *module) const = 0;

        virtual Version detectVersion() const = 0;
    };
}

#endif
