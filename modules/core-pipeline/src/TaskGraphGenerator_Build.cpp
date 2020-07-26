
#include <bok/core/pipeline/TaskGraphGenerator_Build.hpp>

#include <cassert>
#include <iostream>
#include <bok/core/Dag.hpp>
#include <bok/core/DagNode.hpp>
#include <bok/core/Command.hpp>
#include <bok/core/Logger.hpp>
#include <bok/core/package/Package.hpp>
#include <bok/core/package/Module.hpp>
#include <bok/core/package/Source.hpp>
#include <bok/core/toolchain/Toolchain.hpp>
#include <bok/core/toolchain/Compiler.hpp>
#include <bok/core/toolchain/CompileOptions.hpp>
#include <bok/core/toolchain/Linker.hpp>
#include <bok/core/pipeline/BuildCache.hpp>

namespace bok {
    //class BuildCacheUpdateCommand : public Command {
    //public:
    //    BuildCacheUpdateCommand(BuildCache *buildCache, const boost::filesystem::path &sourceFilePath) {
    //        this->buildCache = buildCache;
    //        this->sourceFilePath = sourceFilePath;
    //    }
    //
    //    virtual void execute() override {
    //        buildCache->markAsBuilt(sourceFilePath);
    //    }
    //
    //private:
    //    BuildCache *buildCache = nullptr;
    //    boost::filesystem::path sourceFilePath;
    //};


    class PathVertexMapper {
    public:
        PathVertexMapper(TaskGraph &graph_) : graph(graph_) {}

        std::size_t getVD(const boost::filesystem::path &path) {
            if (auto it = pathVertexMap.find(path); it != pathVertexMap.end()) {
                return it->second;
            }

            const auto objectFileVD = boost::add_vertex(graph);
            graph[objectFileVD].filePath = path;

            pathVertexMap[path] = objectFileVD;

            return objectFileVD;
        }

    private:
        TaskGraph &graph;
        std::map<boost::filesystem::path, std::size_t> pathVertexMap;
    };


    TaskGraph TaskGraphGenerator_Build::generate(Toolchain* toolchain, Module* module) const {
        assert(toolchain);
        assert(module);
        assert(toolchain->enumerateLinkers().size() > 0);
        assert(toolchain->enumerateCompilers().size() > 0);

        const Linker* linker = toolchain->enumerateLinkers()[0];
        assert(linker);

        LinkInput linkInput;
        linkInput.moduleName = module->getName();
        linkInput.moduleType = LinkerModuleType::CliApplication;
        linkInput.outputPath = module->getPath().string();

        TaskGraph graph;
        PathVertexMapper mapper {graph};

        const auto moduleVD = mapper.getVD(module->getPath() / module->getName());
        graph[moduleVD].label = graph[moduleVD].filePath.filename().string();

        for (Source *source : module->getSources()) {
            const Compiler* compiler = toolchain->enumerateCompilers()[0];
            assert(compiler);

            // prepare the compile instruction
            CompileInput compileInput;
            compileInput.sourceFilePath = source->getFilePath().string();
            compileInput.runtimeLink = CompileRuntimeLink::DynamicDebug;
            compileInput.debugInformation = true;
            compileInput.optimization = CompileOptimization::NoOptimization;
            compileInput.language = CompileLanguage::Cplusplus17;
            compileInput.targetArchitecture = CompileTargetArchitecture::Native;
            compileInput.outputFilePath = source->getFilePath().string() + ".obj";
            
            // gather the object file name
            linkInput.objectFiles.push_back(compileInput.outputFilePath);

            // gather the compile output commands
            CompileOutput compileOutput = compiler->generateCompileOutput(compileInput);

            // object file node
            const auto objectFileVD = mapper.getVD(compileInput.outputFilePath);
            graph[objectFileVD].label = graph[objectFileVD].filePath.filename().string();

            // source file node 
            const auto sourceFileVD = mapper.getVD(compileInput.sourceFilePath);
            graph[sourceFileVD].label = graph[sourceFileVD].filePath.filename().string();

            // mark that object depends on source to generate
            const auto objectED = boost::add_edge(objectFileVD, sourceFileVD, graph);
            graph[objectED.first].label = "compile[C++] " + boost::filesystem::path{ compileInput.sourceFilePath }.filename().string();
            graph[objectED.first].command = compileOutput.compileCommand;

            // trespass the dependency information, from include headers to the object file
            for (const std::string &includeFile : compileOutput.dependencyHeaders) {
                const auto includeFileVD = mapper.getVD(includeFile);
                graph[includeFileVD].label = graph[includeFileVD].filePath.filename().string();

                boost::add_edge(objectFileVD, includeFileVD, graph);
            }

            // mark also that the module file depends on the object files
            boost::add_edge(moduleVD, objectFileVD, graph);
        }

        LinkOutput linkOutput = linker->generateLinkOutput(linkInput);

        // create a edge, from the module to a physical (binary module)
        const auto binaryModuleNode = mapper.getVD(graph[moduleVD].filePath.string() + ".exe");
        const auto binaryModuleEdge = boost::add_edge(binaryModuleNode, moduleVD, graph);

        graph[binaryModuleEdge.first].label = "link [C++] " + graph[binaryModuleNode].filePath.filename().string();
        graph[binaryModuleEdge.first].command = linkOutput.linkCommand;

        return graph;
    }
}
