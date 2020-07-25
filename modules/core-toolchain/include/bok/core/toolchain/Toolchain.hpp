
#pragma once 

#include <vector>

namespace bok {
    class Compiler;
    class Linker;
    class Source;
    class Component;

    struct Version;

    class Toolchain {
    public:
        virtual ~Toolchain();

        virtual std::vector<Compiler*> enumerateCompilers() const = 0;

        virtual std::vector<Linker*> enumerateLinkers() const = 0;

        virtual Version detectVersion() const = 0;

        [[deprecated]]
        virtual const Compiler* selectCompiler(const Source* source) const {
            return nullptr;
        }

        [[deprecated]]
        virtual const Linker* selectLinker(const Component* component) const {
            return nullptr;
        }
    };
}
