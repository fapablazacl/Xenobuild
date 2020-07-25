
#include <bok/core/toolchain/ToolchainFactoryHardcoded.hpp>

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


    struct ToolchainFactoryHardcoded::Impl {
        std::map<std::string, boost::filesystem::path> installationPath;
        std::vector<std::unique_ptr<Toolchain>> toolchains;
    };

    ToolchainFactoryHardcoded::ToolchainFactoryHardcoded() : impl(new ToolchainFactoryHardcoded::Impl()) {

    }

    ToolchainFactoryHardcoded::~ToolchainFactoryHardcoded() {}

    Toolchain* ToolchainFactoryHardcoded::createToolchain(const std::string &toolchainId) {
        return nullptr;
    }
}
