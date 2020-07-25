
#ifndef __BOK_TOOLCHAIN_COMPILER_HPP__
#define __BOK_TOOLCHAIN_COMPILER_HPP__

#include <vector>
#include <iostream>
#include <boost/filesystem/path.hpp>
#include <bok/core/Command.hpp>

namespace bok {

    enum class CompileLanguage {
        Cplusplus98,
        Cplusplus03,
        Cplusplus11,
        Cplusplus14,
        Cplusplus17,
        Cplusplus20
    };

    enum class CompileOptimization {
        NoOptimization,
        FavorSize,
        FavorSpeed,
        FullSpeed,
    };

    enum class CompileRuntimeLink {
        StaticDebug,
        StaticRelease,
        DynamicDebug,
        DynamicRelease
    };

    enum class CompileTargetArchitecture {
        Native,
        x86_32,
        x86_64,
    };

    struct CompileInput {
        std::string sourceFilePath;
        std::string outputFilePath;
        CompileLanguage language;
        CompileOptimization optimization;
        CompileTargetArchitecture targetArchitecture;
        CompileRuntimeLink runtimeLink;
        std::vector<std::string> includePaths;
        std::map<std::string, std::string> definitions;
        bool debugInformation;
    };

    struct CompileOptions {};
    class Dag;
    class DagNode;

    struct CompileOutput {
        CommandData compileCommand;
        std::vector<std::string> dependencyHeaders;

        [[deprecated]]
        DagNode *node = nullptr;
    };

    class Source;
    class Compiler {
    public:
        virtual ~Compiler();

        virtual CompileOutput generateCompileOutput(const CompileInput &input) const = 0;

        [[deprecated]]
        virtual bool isSourceLinkable(const Source *source) const {
            return false;
        }

        [[deprecated]]
        virtual CompileOutput compile(Dag* dag, const boost::filesystem::path& outputPath, const Source* source, const CompileOptions& options) const {
            return {};
        }

        [[deprecated]]
        virtual std::vector<boost::filesystem::path> computeDependencies(const boost::filesystem::path& outputPath, const Source* source, const CompileOptions& options) const {
            return {};
        }

        [[deprecated]]
        virtual boost::filesystem::path compileOutputFile(const boost::filesystem::path& outputPath, const Source* source, const CompileOptions& options) const {
            return {};
        }
    };
}

#endif
