
#pragma once 

#include <vector>

namespace bok {
    class Compiler;
    class Linker;
    class Source;
    class Module;

    struct Version;

    /**
     * @brief An ID to a Compiler Type. Depends on the language and the specific toolchain.
     */
    enum class CompilerType {
        Source,
        // Windows_Resource
    };

    /**
     * @brief An ID to a Linker Type. Depends on the language and the specific toolchain.
     */
    enum class LinkerType {
        Binary,
        // Archiver
    };

    class Toolchain {
    public:
        virtual ~Toolchain();

        virtual std::vector<Compiler*> enumerateCompilers() const = 0;

        virtual std::vector<Linker*> enumerateLinkers() const = 0;

        virtual Version detectVersion() const = 0;

        virtual Compiler* getCompiler(const CompilerType type) const = 0;

        virtual Linker* getLinker(const LinkerType type) const = 0;

        [[deprecated]]
        virtual const Compiler* selectCompiler(const Source* source) const {
            return nullptr;
        }

        [[deprecated]]
        virtual const Linker* selectLinker(const Module* component) const {
            return nullptr;
        }
    };
}
