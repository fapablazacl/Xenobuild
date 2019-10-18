
#include <borc/services/BuildServiceImpl.hpp>

#include <iostream>
#include <borc/utility/Dag.hpp>
#include <borc/utility/DagNode.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Source.hpp>
#include <borc/model/Command.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/toolchain/Compiler.hpp>
#include <borc/toolchain/CompileOptions.hpp>
#include <borc/toolchain/Linker.hpp>
#include <borc/services/LoggingService.hpp>
#include <borc/build/BuildCache.hpp>

namespace borc {
    class BuildCacheUpdateCommand : public Command {
    public:
        BuildCacheUpdateCommand(BuildCache *buildCache, const boost::filesystem::path &sourceFilePath) {
            this->buildCache = buildCache;
            this->sourceFilePath = sourceFilePath;
        }

        virtual void execute() override {
            buildCache->markAsBuilt(sourceFilePath);
        }

    private:
        BuildCache *buildCache = nullptr;
        boost::filesystem::path sourceFilePath;
    };


    BuildServiceImpl::BuildServiceImpl(const boost::filesystem::path &basePath, const boost::filesystem::path &outputPath, Toolchain *toolchain, BuildCache* buildCache, LoggingService *logger) {
        this->basePath = basePath;
        this->outputPath = outputPath;
        this->toolchain = toolchain;
        this->buildCache = buildCache;
        this->logger = logger;
    }


    BuildServiceImpl::~BuildServiceImpl() {}


    std::unique_ptr<Dag> BuildServiceImpl::createBuildDag(Package *package) {
        auto dag = std::make_unique<Dag>();

        for (Module *module : package->getModules()) {
            const Linker *linker = toolchain->selectLinker(module);

            if (!linker) {
                if (logger) {
                    logger->warn("Couldn't find a linker using the current toolchain");
                }

                continue;
            }

            module->rescanSources(basePath);

            const CompileOptions compileOptions = this->computeCompileOptions(module);

            DagNode *moduleDagNode = dag->createNode();

            std::vector<boost::filesystem::path> objectFiles;

            for (Source *source : module->getSources()) {
                const Compiler *compiler = toolchain->selectCompiler(source);

                if (!compiler || !buildCache->needsRebuild(source->getFilePath())) {
                    continue;
                }

                CompileOutput compileOutput = compiler->compile(dag.get(), outputPath, source, compileOptions);
                objectFiles.push_back(compileOutput.outputFileRelativePath);

                Command *buildCacheUpdateCommand = this->createBuildCacheUpdateCommand(source->getFilePath());
                DagNode *buildCacheUpdate = dag->createNode(buildCacheUpdateCommand);
                buildCacheUpdate->appendDependency(compileOutput.node);

                moduleDagNode->appendDependency(buildCacheUpdate);
            }

            LinkOutput linkOutput = linker->link(outputPath, package, module, objectFiles);
            
            moduleDagNode->setCommand(linkOutput.command);

            dag->getRoot()->appendDependency(moduleDagNode);
        }

        return dag;
    }


    CompileOptions BuildServiceImpl::computeCompileOptions(const Module *module) const {
        CompileOptions options;

        for (const boost::filesystem::path &includePath : module->getIncludePaths()) {
            const auto resolvedIncludePath = basePath / module->getPath() / includePath;
            options.includePaths.push_back(resolvedIncludePath.string());
        }

        // TODO: Compute this recursively
        // compute include paths for dependent modules
        for (const Module *dependentModule : module->getDependencies()) {
            CompileOptions dependentOptions = this->computeCompileOptions(dependentModule);

            options.mergeWith(dependentOptions);
        }

        return options;
    }

    class PathVertexMapper {
    public:
        PathVertexMapper(DependencyGraph &graph_) : graph(graph_) {}

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
        DependencyGraph &graph;
        std::map<boost::filesystem::path, std::size_t> pathVertexMap;
    };

    DependencyGraph BuildServiceImpl::computeDependencyGraph(Module *module) const {
        if (!module) {
            throw std::runtime_error("Supplied module object is a null pointer.");
        }

        const Linker *linker = toolchain->selectLinker(module);
        if (!linker) {
            throw std::runtime_error("There is no linker for the supplied module.");
        }

        DependencyGraph graph;
        PathVertexMapper mapper {graph};

        const auto moduleVD = mapper.getVD(module->getPath() / module->getName());
        graph[moduleVD].label = graph[moduleVD].filePath.filename().string();

        const CompileOptions compileOptions = this->computeCompileOptions(module);
        module->rescanSources(basePath);

        for (Source *source : module->getSources()) {
            const Compiler *compiler = toolchain->selectCompiler(source);

            if (!compiler) {
                // throw std::runtime_error("There is no compiler for the current source.");
                continue;
            }

            const auto objectFileVD = mapper.getVD(compiler->compileOutputFile(outputPath, source, compileOptions));
            graph[objectFileVD].label = graph[objectFileVD].filePath.filename().string();

            const auto sourceFileVD = mapper.getVD(source->getFilePath());
            graph[sourceFileVD].label = graph[sourceFileVD].filePath.filename().string();

            const auto objectED = boost::add_edge(objectFileVD, sourceFileVD, graph);
            graph[objectED.first].label = "Hola";
            graph[objectED.first].command = "Hola";

            const auto includeFiles = compiler->computeDependencies(outputPath, source, compileOptions);
            for (const boost::filesystem::path &includeFile : includeFiles) {
                const auto includeFileVD = mapper.getVD(includeFile);
                graph[includeFileVD].label = graph[includeFileVD].filePath.filename().string();

                boost::add_edge(objectFileVD, includeFileVD, graph);
            }

            boost::add_edge(moduleVD, objectFileVD, graph);
        }

        return graph;
    }


    Command* BuildServiceImpl::createBuildCacheUpdateCommand(const boost::filesystem::path &filePath) {
        auto command = new BuildCacheUpdateCommand(this->buildCache, filePath);

        commandStorage.emplace_back(command);

        return command;
    }
}
