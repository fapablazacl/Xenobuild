
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
#include <bok/core/toolchain/Compiler.hpp>
#include <bok/core/toolchain/CompileOptions.hpp>
#include <bok/core/toolchain/Linker.hpp>
#include <bok/core/pipeline/BuildCache.hpp>


namespace bok {
    class PathVertexMapper {
    public:
        PathVertexMapper(TaskGraph &graph_) : graph(graph_) {}

        std::size_t getVD(const boost::filesystem::path &path) {
            if (auto it = pathVertexMap.find(path); it != pathVertexMap.end()) {
                return it->second;
            }

            const auto objectFileVD = boost::add_vertex(graph.adjacencyList);
            graph.adjacencyList[objectFileVD].filePath = path;

            pathVertexMap[path] = objectFileVD;

            return objectFileVD;
        }

    private:
        TaskGraph &graph;
        std::map<boost::filesystem::path, std::size_t> pathVertexMap;
    };


    static CompileInput complementCompileInput(CompileInput input, const PrebuiltPackageBuildData& buildData) {
        input.includePaths.insert(input.includePaths.end(), buildData.includePaths.begin(), buildData.includePaths.end());

        return input;
    }

    static LinkInput complementLinkInput(LinkInput input, const PrebuiltPackageBuildData& buildData) {
        input.libraryPaths.insert(input.libraryPaths.end(), buildData.libraryPaths.begin(), buildData.libraryPaths.end());
        input.libraries.insert(input.libraries.end(), buildData.libraries.begin(), buildData.libraries.end());

        return input;
    }

    TaskGraphGenerator_Build::TaskGraphGenerator_Build(const WildcardClassifier<CompilerType>& sourceClassifier) 
        : sourceClassifier(sourceClassifier) {}


    TaskGraph TaskGraphGenerator_Build::generate(Toolchain* toolchain, Module* module) const {
        assert(toolchain);
        assert(module);
        assert(toolchain->enumerateLinkers().size() > 0);
        assert(toolchain->enumerateCompilers().size() > 0);

        // TODO: Refactor later into a more scalable solution (dependency management)
        const auto winsdkBuildData = toolchain->getPackageBuildData("winsdk");

        const Linker* linker = pickLinker(toolchain, module);
        
        LinkInput linkInput;
        linkInput.moduleName = module->getName();
        linkInput.moduleType = LinkerModuleType::CliApplication;
        linkInput.outputPath = module->getPath().string();

        if (winsdkBuildData) {
            linkInput = complementLinkInput(linkInput, *winsdkBuildData);
        }

        TaskGraph graph;
        PathVertexMapper mapper {graph};

        const auto moduleVD = mapper.getVD(module->getPath() / module->getName());
        graph.adjacencyList[moduleVD].label = graph.adjacencyList[moduleVD].filePath.filename().string();

        for (Source *source : module->getSources()) {
            const auto compiler = pickCompiler(toolchain, source);

            if (!compiler) {
                continue;
            }

            // prepare the compile instruction
            CompileInput compileInput;
            compileInput.sourceFilePath = source->getFilePath().string();
            compileInput.runtimeLink = CompileRuntimeLink::DynamicDebug;
            compileInput.debugInformation = true;
            compileInput.optimization = CompileOptimization::NoOptimization;
            compileInput.language = CompileLanguage::Cplusplus17;
            compileInput.targetArchitecture = CompileTargetArchitecture::Native;
            compileInput.outputFilePath = source->getFilePath().string() + ".obj";

            if (winsdkBuildData) {
                compileInput = complementCompileInput(compileInput, *winsdkBuildData);
            }

            // gather the object file name
            linkInput.objectFiles.push_back(compileInput.outputFilePath);

            // gather the compile output commands
            CompileOutput compileOutput = compiler->generateCompileOutput(compileInput);

            // object file node
            const auto objectFileVD = mapper.getVD(compileInput.outputFilePath);
            graph.adjacencyList[objectFileVD].label = graph.adjacencyList[objectFileVD].filePath.filename().string();

            // source file node 
            const auto sourceFileVD = mapper.getVD(compileInput.sourceFilePath);
            graph.adjacencyList[sourceFileVD].label = graph.adjacencyList[sourceFileVD].filePath.filename().string();

            // mark that object depends on source to generate
            const auto objectED = boost::add_edge(objectFileVD, sourceFileVD, graph.adjacencyList);
            graph.adjacencyList[objectED.first].label = "compile [C++] " + compileInput.sourceFilePath.filename().string();
            graph.adjacencyList[objectED.first].command = compileOutput.compileCommand;
            graph.adjacencyList[objectED.first].commandRequired = true;

            // trespass the dependency information, from include headers to the object file
            for (const auto &includeFile : compileOutput.dependencyHeaders) {
                const auto includeFileVD = mapper.getVD(includeFile);
                graph.adjacencyList[includeFileVD].label = graph.adjacencyList[includeFileVD].filePath.filename().string();

                boost::add_edge(objectFileVD, includeFileVD, graph.adjacencyList);
            }

            // mark also that the module file depends on the object files
            boost::add_edge(moduleVD, objectFileVD, graph.adjacencyList);
        }

        LinkOutput linkOutput = linker->generateLinkOutput(linkInput);

        // create a edge, from the module to a physical (binary module)
        // TODO: The ".exe" addition must be refactored to a "target" platform management code (maybe via DI in this class)
        const auto binaryModuleNode = mapper.getVD(graph.adjacencyList[moduleVD].filePath.string() + ".exe");   
        const auto binaryModuleEdge = boost::add_edge(binaryModuleNode, moduleVD, graph.adjacencyList);

        graph.adjacencyList[binaryModuleEdge.first].label = "link [C++] " + graph.adjacencyList[binaryModuleNode].filePath.filename().string();
        graph.adjacencyList[binaryModuleEdge.first].command = linkOutput.linkCommand;
        graph.adjacencyList[binaryModuleEdge.first].commandRequired = true;

        graph.moduleVertexDescriptor = binaryModuleNode;

        return graph;
    }

    const Compiler* TaskGraphGenerator_Build::pickCompiler(const Toolchain* toolchain, const Source* source) const {
        assert(toolchain);
        assert(source);

        const auto compilerType = sourceClassifier.getFileType(source->getFilePath().string());

        if (!compilerType) {
            return nullptr;
        }

        return toolchain->getCompiler(*compilerType);
    }

    const Linker* TaskGraphGenerator_Build::pickLinker(const Toolchain* toolchain, const Module* module) const {
        assert(toolchain);
        assert(module);

        const Linker* linker = toolchain->enumerateLinkers()[0];
        assert(linker);

        return linker;
    }
}
