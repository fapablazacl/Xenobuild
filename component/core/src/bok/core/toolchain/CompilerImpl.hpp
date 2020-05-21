
#ifndef __BOK_TOOLCHAIN_COMPILERIMPL_HPP__
#define __BOK_TOOLCHAIN_COMPILERIMPL_HPP__

#include <bok/core/toolchain/Compiler.hpp>

#include <vector>
#include <iostream>
#include <boost/filesystem/path.hpp>

namespace bok {
    class DagNode;
    class Package;
    class Component;
    class Command;
    class CommandFactory;

    class Source;
    class Dag;
    class DagNode;
    class CompilerImpl : public Compiler {
    public:
        /*
        struct LanguageDialectSwitch {
            std::string name;
            std::string flag;
        };


        struct OptimizationLevelSwitch {
            std::string name;
            std::string flag;
        };
        */

        struct Switches {
            std::string compile;
            std::string objectFileOutput;
            std::string includeDebug;
            std::string zeroOptimization;
            std::string includePath;
            std::string generateDependencies;

            // std::vector<LanguageDialectSwitch> dialects;
            // std::vector<OptimizationLeve
        };

        struct BuildRuleInput {
            std::string fileType;
        };

        struct BuildRuleOutput {
            std::string fileType;
            std::string fileName;
        };

        struct BuildRule {
            BuildRuleInput input;
            BuildRuleOutput output;
        };
        
    public:
        explicit CompilerImpl(
            CommandFactory *commandFactory, 
            const boost::filesystem::path &commandPath, 
            const Switches &switches,
            const std::vector<BuildRule> &buildRules
        );

        virtual ~CompilerImpl();

        virtual bool isSourceLinkable(const Source *source) const override;

        virtual CompileOutput compile(Dag *dag, const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const override;

        virtual std::vector<boost::filesystem::path> computeDependencies(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const override;

        virtual boost::filesystem::path compileOutputFile(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const override;

    private:
        struct Private;
        Private *m_impl = nullptr;
    };
}

#endif
