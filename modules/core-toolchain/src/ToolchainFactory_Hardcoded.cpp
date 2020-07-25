
#include <bok/core/toolchain/ToolchainFactory_Hardcoded.hpp>

#include <map>
#include <vector>

#include <bok/core/toolchain/Toolchain.hpp>
#include <bok/core/toolchain/Compiler.hpp>
#include <bok/core/toolchain/Linker.hpp>

namespace bok {
    /*
    class ToolchainGCC : public Toolchain {
    protected:
        class CompilerGCC : public Compiler {
        public:
        virtual bool isSourceLinkable(const Source *source) const {
            return false;
        }

        virtual CompileOutput compile(Dag *dag, const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const = 0;

        virtual std::vector<boost::filesystem::path> computeDependencies(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const = 0;

        virtual boost::filesystem::path compileOutputFile(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const = 0;
        };

        class LinkerGCC : public Linker {
        public:

        };

        class LinkerAR : public Linker {
        public:

        };

    public:
        CompilerGCC compilerGCC;
        LinkerGCC linkerGCC;
        LinkerAR linkerAR;
    };


    class ToolchainVC : public Toolchain {
    public:


    };
    */


    struct ToolchainFactory_Hardcoded::Impl {
        std::map<std::string, boost::filesystem::path> installationPath;
        std::vector<std::unique_ptr<Toolchain>> toolchains;
    };

    ToolchainFactory_Hardcoded::ToolchainFactory_Hardcoded() : impl(new ToolchainFactory_Hardcoded::Impl()) {

    }

    ToolchainFactory_Hardcoded::~ToolchainFactory_Hardcoded() {}

    Toolchain* ToolchainFactory_Hardcoded::createToolchain(const std::string &toolchainId) {
        return nullptr;
    }
}
