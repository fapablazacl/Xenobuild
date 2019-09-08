
#include <borc/services/BuildServiceImpl.hpp>

#include <iostream>
#include <borc/utility/Dag.hpp>
#include <borc/utility/DagNode.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/Artifact.hpp>
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

        for (Artifact *artifact : package->getArtifacts()) {
            const Linker *linker = toolchain->selectLinker(artifact);

            if (!linker) {
                if (logger) {
                    logger->warn("Couldn't find a linker using the current toolchain");
                }

                continue;
            }

            artifact->rescanSources(basePath);

            const CompileOptions compileOptions = this->computeCompileOptions(artifact);

            DagNode *artifactDagNode = dag->createNode();

            std::vector<boost::filesystem::path> objectFiles;

            for (Source *source : artifact->getSources()) {
                const Compiler *compiler = toolchain->selectCompiler(source);

                if (!compiler || !buildCache->needsRebuild(source->getFilePath())) {
                    continue;
                }

                CompileOutput compileOutput = compiler->compile(dag.get(), outputPath, source, compileOptions);
                objectFiles.push_back(compileOutput.outputFileRelativePath);

                Command *buildCacheUpdateCommand = this->createBuildCacheUpdateCommand(source->getFilePath());
                DagNode *buildCacheUpdate = dag->createNode(buildCacheUpdateCommand);
                buildCacheUpdate->appendDependency(compileOutput.node);

                artifactDagNode->appendDependency(buildCacheUpdate);
            }

            LinkOutput linkOutput = linker->link(outputPath, package, artifact, objectFiles);
            
            artifactDagNode->setCommand(linkOutput.command);

            dag->getRoot()->appendDependency(artifactDagNode);
        }

        return dag;
    }


    CompileOptions BuildServiceImpl::computeCompileOptions(const Artifact *artifact) const {
        CompileOptions options;

        for (const boost::filesystem::path &includePath : artifact->getIncludePaths()) {
            const auto resolvedIncludePath = basePath / artifact->getPath() / includePath;
            options.includePaths.push_back(resolvedIncludePath.string());
        }

        // TODO: Compute this recursively
        // compute include paths for dependent artifacts
        for (const Artifact *dependentArtifact : artifact->getDependencies()) {
            CompileOptions dependentOptions = this->computeCompileOptions(dependentArtifact);

            options.mergeWith(dependentOptions);
        }

        return options;
    }


    DependencyGraph BuildServiceImpl::computeDependencyGraph(Artifact *artifact) const {
        if (!artifact) {
            throw std::runtime_error("Supplied artifact object is a null pointer.");
        }

        const Linker *linker = toolchain->selectLinker(artifact);
        if (!linker) {
            throw std::runtime_error("There is no linker for the supplied artifact.");
        }

        DependencyGraph graph;

        auto artifactVD = boost::add_vertex(graph);
        graph[artifactVD].filePath = artifact->getPath() / artifact->getName();

        const CompileOptions compileOptions = this->computeCompileOptions(artifact);
        artifact->rescanSources(basePath);

        for (Source *source : artifact->getSources()) {
            const Compiler *compiler = toolchain->selectCompiler(source);

            if (!compiler) {
                // throw std::runtime_error("There is no compiler for the current source.");
                continue;
            }

            const auto objectFileVD = boost::add_vertex(graph);
            graph[objectFileVD].filePath = compiler->compiteOutputFile(outputPath, source, compileOptions);

            const auto sourceFileVD = boost::add_vertex(graph);
            graph[sourceFileVD].filePath = source->getFilePath();

            boost::add_edge(objectFileVD, sourceFileVD, graph);

            const auto includeFiles = compiler->computeDependencies(outputPath, source, compileOptions);
            for (const boost::filesystem::path &includeFile : includeFiles) {
                const auto includeFileVD = boost::add_vertex(graph);
                graph[includeFileVD].filePath = includeFile;

                boost::add_edge(objectFileVD, includeFileVD, graph);
            }

            boost::add_edge(artifactVD, objectFileVD, graph);
        }

        return graph;
    }


    Command* BuildServiceImpl::createBuildCacheUpdateCommand(const boost::filesystem::path &filePath) {
        auto command = new BuildCacheUpdateCommand(this->buildCache, filePath);

        commandStorage.emplace_back(command);

        return command;
    }
}
