
#include <bok/core/pipeline/BuildTaskGraphGenerator.hpp>

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


    struct BuildTaskGraphGenerator::Private {
        boost::filesystem::path basePath;
        boost::filesystem::path outputPath;
        Toolchain *toolchain = nullptr;
        BuildCache *buildCache = nullptr;
        Logger *logger = nullptr;
        std::vector<std::unique_ptr<Command>> commandStorage;


        CompileOptions computeCompileOptions(const Component *component) const {
            CompileOptions options;

            for (const boost::filesystem::path &includePath : component->getIncludePaths()) {
                const auto resolvedIncludePath = basePath / component->getPath() / includePath;

                //! TODO: Rethink the design of this class, to make it work with the Compiler class.
                // options.includePaths.push_back(resolvedIncludePath.string());
            }

            // TODO: Compute this recursively
            // compute include paths for dependent modules
            for (const Component *dependentModule : component->getDependencies()) {
                CompileOptions dependentOptions = this->computeCompileOptions(dependentModule);

                //! TODO: Rethink the design of this class, to make it work with the new design of the Compiler class.
                // options.mergeWith(dependentOptions);
            }

            return options;
        }


        Command* createBuildCacheUpdateCommand(const boost::filesystem::path &filePath) {
            auto command = new BuildCacheUpdateCommand(this->buildCache, filePath);

            commandStorage.emplace_back(command);

            return command;
        }
    };


    BuildTaskGraphGenerator::BuildTaskGraphGenerator(const boost::filesystem::path &basePath, const boost::filesystem::path &outputPath, Toolchain *toolchain, BuildCache* buildCache, Logger *logger) 
            : m_impl(new BuildTaskGraphGenerator::Private()) {
        m_impl->basePath = basePath;
        m_impl->outputPath = outputPath;
        m_impl->toolchain = toolchain;
        m_impl->buildCache = buildCache;
        m_impl->logger = logger;
    }


    BuildTaskGraphGenerator::~BuildTaskGraphGenerator() {
        delete m_impl;
    }


    std::unique_ptr<Dag> BuildTaskGraphGenerator::createBuildDag(Package *package) {
        auto dag = std::make_unique<Dag>();

        for (Component *component : package->getModules()) {
            const Linker *linker = m_impl->toolchain->selectLinker(component);

            if (! linker) {
                if (m_impl->logger) {
                    m_impl->logger->warn("Couldn't find a linker for component " + component->getName() + " using the current toolchain");
                }

                continue;
            }

            component->rescanSources(m_impl->basePath);

            const CompileOptions compileOptions = m_impl->computeCompileOptions(component);

            DagNode *moduleDagNode = dag->createNode();

            std::vector<boost::filesystem::path> objectFiles;

            for (Source *source : component->getSources()) {
                const Compiler *compiler = m_impl->toolchain->selectCompiler(source);

                if (! compiler) {
                    continue;
                }
                
                CompileOutput compileOutput = compiler->compile(dag.get(), m_impl->outputPath, source, compileOptions);

                //! TODO: Rethink the design of this class, to make it work with the new design of the Compiler class.
                // objectFiles.push_back(compileOutput.outputFileRelativePath);

                if (! m_impl->buildCache->needsRebuild(source->getFilePath())) {
                    continue;
                }

                Command *buildCacheUpdateCommand = m_impl->createBuildCacheUpdateCommand(source->getFilePath());
                DagNode *buildCacheUpdate = dag->createNode(buildCacheUpdateCommand);
                buildCacheUpdate->appendDependency(compileOutput.node);

                moduleDagNode->appendDependency(buildCacheUpdate);
            }

            LinkOutput linkOutput = linker->link(m_impl->outputPath, package, component, objectFiles);
            
            moduleDagNode->setCommand(linkOutput.command);

            dag->getRoot()->appendDependency(moduleDagNode);
        }

        return dag;
    }


    TaskGraph BuildTaskGraphGenerator::generate(Component *component) const {
        assert(component);

        const Linker *linker = m_impl->toolchain->selectLinker(component);
        if (!linker) {
            throw std::runtime_error("There is no linker for the supplied component.");
        }

        TaskGraph graph;
        PathVertexMapper mapper {graph};

        const auto moduleVD = mapper.getVD(component->getPath() / component->getName());
        graph[moduleVD].label = graph[moduleVD].filePath.filename().string();

        const CompileOptions compileOptions = m_impl->computeCompileOptions(component);
        component->rescanSources(m_impl->basePath);

        for (Source *source : component->getSources()) {
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


    TaskGraph BuildTaskGraphGenerator::generate(Package *package) const {
        assert(package);

        return {};
    }
}
