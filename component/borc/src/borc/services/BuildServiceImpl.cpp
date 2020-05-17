
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

namespace bok {
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


    struct BuildServiceImpl::Private {
        boost::filesystem::path basePath;
        boost::filesystem::path outputPath;
        Toolchain *toolchain = nullptr;
        BuildCache *buildCache = nullptr;
        LoggingService *logger = nullptr;
        std::vector<std::unique_ptr<Command>> commandStorage;


        CompileOptions computeCompileOptions(const Module *module) const {
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


        Command* createBuildCacheUpdateCommand(const boost::filesystem::path &filePath) {
            auto command = new BuildCacheUpdateCommand(this->buildCache, filePath);

            commandStorage.emplace_back(command);

            return command;
        }
    };


    BuildServiceImpl::BuildServiceImpl(const boost::filesystem::path &basePath, const boost::filesystem::path &outputPath, Toolchain *toolchain, BuildCache* buildCache, LoggingService *logger) 
            : m_impl(new BuildServiceImpl::Private()) {
        m_impl->basePath = basePath;
        m_impl->outputPath = outputPath;
        m_impl->toolchain = toolchain;
        m_impl->buildCache = buildCache;
        m_impl->logger = logger;
    }


    BuildServiceImpl::~BuildServiceImpl() {
        delete m_impl;
    }


    std::unique_ptr<Dag> BuildServiceImpl::createBuildDag(Package *package) {
        auto dag = std::make_unique<Dag>();

        for (Module *module : package->getModules()) {
            const Linker *linker = m_impl->toolchain->selectLinker(module);

            if (! linker) {
                if (m_impl->logger) {
                    m_impl->logger->warn("Couldn't find a linker for module " + module->getName() + " using the current toolchain");
                }

                continue;
            }

            module->rescanSources(m_impl->basePath);

            const CompileOptions compileOptions = m_impl->computeCompileOptions(module);

            DagNode *moduleDagNode = dag->createNode();

            std::vector<boost::filesystem::path> objectFiles;

            for (Source *source : module->getSources()) {
                const Compiler *compiler = m_impl->toolchain->selectCompiler(source);

                if (! compiler) {
                    continue;
                }
                
                CompileOutput compileOutput = compiler->compile(dag.get(), m_impl->outputPath, source, compileOptions);
                objectFiles.push_back(compileOutput.outputFileRelativePath);

                if (! m_impl->buildCache->needsRebuild(source->getFilePath())) {
                    continue;
                }

                Command *buildCacheUpdateCommand = m_impl->createBuildCacheUpdateCommand(source->getFilePath());
                DagNode *buildCacheUpdate = dag->createNode(buildCacheUpdateCommand);
                buildCacheUpdate->appendDependency(compileOutput.node);

                moduleDagNode->appendDependency(buildCacheUpdate);
            }

            LinkOutput linkOutput = linker->link(m_impl->outputPath, package, module, objectFiles);
            
            moduleDagNode->setCommand(linkOutput.command);

            dag->getRoot()->appendDependency(moduleDagNode);
        }

        return dag;
    }


    DependencyGraph BuildServiceImpl::computeDependencyGraph(Module *module) const {
        if (!module) {
            throw std::runtime_error("Supplied module object is a null pointer.");
        }

        const Linker *linker = m_impl->toolchain->selectLinker(module);
        if (!linker) {
            throw std::runtime_error("There is no linker for the supplied module.");
        }

        DependencyGraph graph;
        PathVertexMapper mapper {graph};

        const auto moduleVD = mapper.getVD(module->getPath() / module->getName());
        graph[moduleVD].label = graph[moduleVD].filePath.filename().string();

        const CompileOptions compileOptions = m_impl->computeCompileOptions(module);
        module->rescanSources(m_impl->basePath);

        for (Source *source : module->getSources()) {
            const Compiler *compiler = m_impl->toolchain->selectCompiler(source);

            if (!compiler) {
                // throw std::runtime_error("There is no compiler for the current source.");
                continue;
            }

            const auto objectFileVD = mapper.getVD(compiler->compileOutputFile(m_impl->outputPath, source, compileOptions));
            graph[objectFileVD].label = graph[objectFileVD].filePath.filename().string();

            const auto sourceFileVD = mapper.getVD(source->getFilePath());
            graph[sourceFileVD].label = graph[sourceFileVD].filePath.filename().string();

            const auto objectED = boost::add_edge(objectFileVD, sourceFileVD, graph);
            graph[objectED.first].label = "Hola";
            graph[objectED.first].command = "Hola";

            const auto includeFiles = compiler->computeDependencies(m_impl->outputPath, source, compileOptions);
            for (const boost::filesystem::path &includeFile : includeFiles) {
                const auto includeFileVD = mapper.getVD(includeFile);
                graph[includeFileVD].label = graph[includeFileVD].filePath.filename().string();

                boost::add_edge(objectFileVD, includeFileVD, graph);
            }

            boost::add_edge(moduleVD, objectFileVD, graph);
        }

        return graph;
    }
}
