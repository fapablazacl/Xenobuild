
#include <borc/services/BuildServiceImpl.hpp>

#include <iostream>
#include <borc/utility/Dag.hpp>
#include <borc/utility/DagNode.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/model/Source.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/toolchain/Compiler.hpp>
#include <borc/toolchain/Linker.hpp>
#include <borc/services/LoggingService.hpp>

namespace borc {
    BuildServiceImpl::BuildServiceImpl(const boost::filesystem::path &basePath, const boost::filesystem::path &outputPath, Toolchain *toolchain, LoggingService *logger) {
        this->basePath = basePath;
        this->outputPath = outputPath;
        this->toolchain = toolchain;
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

            artifact->rebuildSources(basePath);

            const CompileOptions compileOptions = this->computeCompileOptions(artifact);

            DagNode *artifactDagNode = dag->createNode();

            const std::vector<Source*> sources = artifact->getSources();
            std::vector<boost::filesystem::path> objectFiles;

            for (Source *source : sources) {
                const Compiler *compiler = toolchain->selectCompiler(source);

                if (! compiler) {
                    /*
                    if (logger) {
                        std::string msg = "";

                        msg += "Couldn't find a compiler using the current toolchain ";
                        msg += "for the file ";
                        msg += "'" + source->getFilePath().string() + "'";
                        msg += ".";

                        logger->warn(msg);
                    }
                    */

                    continue;
                }

                CompileOutput compileOutput = compiler->compile(dag.get(), outputPath, source, compileOptions);

                objectFiles.push_back(compileOutput.outputFileRelativePath);
                artifactDagNode->previous.push_back(compileOutput.node);
            }

            LinkOutput linkOutput = linker->link(outputPath, package, artifact, objectFiles);
            artifactDagNode->command = linkOutput.command;

            dag->getRoot()->previous.push_back(artifactDagNode);
        }

        return dag;
    }


    CompileOptions BuildServiceImpl::computeCompileOptions(const Artifact *artifact) const {
        CompileOptions options;

        for (const boost::filesystem::path &includePath : artifact->getIncludePaths()) {
            const auto resolvedIncludePath = basePath / artifact->getPath() / includePath;
            options.includePaths.push_back(resolvedIncludePath.string());
        }

        return options;
    }
}
