
#include <borc/services/BuildServiceImpl.hpp>

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

            DagNode *artifactDagNode = dag->createNode();

            const std::vector<boost::filesystem::path> sourceFiles = artifact->getSourceFiles(basePath);
            std::vector<boost::filesystem::path> objectFiles;

            for (const boost::filesystem::path &sourceFile : sourceFiles) {
                Source source {artifact, sourceFile};

                const Compiler *compiler = toolchain->selectCompiler(&source);

                if (!compiler) {
                    if (logger) {
                        std::string msg = "";

                        msg += "Couldn't find a compiler using the current toolchain ";
                        msg += "for the file ";
                        msg += "'" + sourceFile.string() + "'";
                        msg += ".";

                        logger->warn(msg);
                    }

                    continue;
                }

                CompileOutput compileOutput = compiler->compile(dag.get(), outputPath, &source);

                objectFiles.push_back(compileOutput.outputFileRelativePath);
                artifactDagNode->previous.push_back(compileOutput.node);
            }

            LinkOutput linkOutput = linker->link(package, artifact, objectFiles);
            artifactDagNode->command = linkOutput.command;

            dag->getRoot()->previous.push_back(artifactDagNode);
        }

        return dag;
    }
}
