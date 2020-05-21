
#include "CompilerImpl.hpp"

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <bok/core/package/Source.hpp>
#include <bok/core/package/Module.hpp>
#include <bok/core/package/Package.hpp>
#include <bok/core/toolchain/CompileOptions.hpp>
#include <bok/core/common/Command.hpp>
#include <bok/core/common/CommandFactory.hpp>
#include <bok/core/common/Dag.hpp>
#include <bok/core/common/DagNode.hpp>


namespace bok {
    struct CompilerImpl::Private {
    public:
        CommandFactory *commandFactory = nullptr;
        boost::filesystem::path commandPath;
        Switches switches;
        std::vector<BuildRule> buildRules;

    public:
        std::vector<boost::filesystem::path> computeFileDependencies(const Source *source, const CompileOptions &options) const {
            if (switches.generateDependencies == "") {
                return {};
            }

            const auto sourceFilePath = source->getFilePath();

            std::vector<std::string> commandOptions;

            boost::split(commandOptions, switches.generateDependencies, boost::is_any_of(" "));

            commandOptions.push_back(sourceFilePath.string());

            // compute system include directories
            // for (const std::string &path : configuration.systemIncludePaths) {
            //     const std::string includeOption = switches.includePath + path;
            // 
            //     commandOptions.push_back(includeOption);
            // }

            // compute additional include directories
            for (const std::string &path : options.includePaths) {
                const std::string includeOption = switches.includePath + path;

                commandOptions.push_back(includeOption);
            }

            // add additional compiler options
            // commandOptions.insert(commandOptions.end(), std::begin(configuration.flags), std::end(configuration.flags));

            boost::filesystem::path compilerPath = boost::process::search_path(commandPath);
            boost::process::ipstream pipeStream;
            boost::process::child childProcess {compilerPath, boost::process::args(commandOptions), boost::process::std_out > pipeStream};

            std::string line;
            std::vector<std::string> specs;

            while (pipeStream && std::getline(pipeStream, line) && !line.empty()) {
                specs.push_back(line);
            }

            childProcess.wait();

            // from the third element onwards we have the dependencies ...
            std::vector<boost::filesystem::path> dependencies;
            for (int i=2; i<specs.size(); i++) {
                std::string dependency = specs[i];

                boost::algorithm::replace_all(dependency, "\\", "");
                boost::algorithm::trim(dependency);

                dependencies.push_back(dependency);
            }

            return dependencies;
        }


        Command* createCompileCommand(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
            const auto sourceFilePath = source->getFilePath();
            const auto objectFilePath = this->getObjectFilePath(outputPath, source);

            // TODO: Factor these options 
            std::vector<std::string> commandOptions = {
                switches.zeroOptimization,
                switches.includeDebug,
                switches.compile,
                "\"" + sourceFilePath.string() + "\"",
                switches.objectFileOutput + "\"" + objectFilePath.string() + "\"",
            };

            // compute system include directories
            // for (const std::string &path : configuration.systemIncludePaths) {
            //     const std::string includeOption = switches.includePath + path;
            // 
            //     commandOptions.push_back(includeOption);
            // }

            // compute additional include directories
            for (const std::string &path : options.includePaths) {
                const std::string includeOption = switches.includePath + path;

                commandOptions.push_back(includeOption);
            }

            // compute include paths from the module
            for (const boost::filesystem::path &includePath : source->getModule()->getIncludePaths()) {
                const std::string includeOption = switches.includePath + includePath.string();

                commandOptions.push_back(includeOption);
            }

            // add additional compiler options
            // commandOptions.insert(commandOptions.end(), std::begin(configuration.flags), std::end(configuration.flags));

            return commandFactory->createCommand(commandPath.string(), commandOptions);
        }


        boost::filesystem::path getObjectFilePath(const boost::filesystem::path &outputPath, const Source *source) const {
            const boost::filesystem::path objectFileName = source->getFilePath().filename().string() + ".obj";
            const boost::filesystem::path objectFileParentPath 
                = outputPath 
                / source->getRelativeFilePath().parent_path();

            const boost::filesystem::path objectFilePath = objectFileParentPath / objectFileName;
            
            return objectFilePath;
        }
    };
    

    CompilerImpl::CompilerImpl(CommandFactory *commandFactory, const boost::filesystem::path &commandPath, const CompilerImpl::Switches &switches, const std::vector<CompilerImpl::BuildRule> &buildRules) : m_impl(new CompilerImpl::Private{}) {
        m_impl->commandFactory = commandFactory;
        m_impl->commandPath = commandPath;
        m_impl->switches = switches;
        m_impl->buildRules = buildRules;
    }


    CompilerImpl::~CompilerImpl() {
        delete m_impl;
    }


    bool CompilerImpl::isSourceLinkable(const Source *source) const {
        return true;
    }


    CompileOutput CompilerImpl::compile(Dag *dag, const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
        // ******************
        m_impl->computeFileDependencies(source, options);

        const boost::filesystem::path outputFileRelativePath = m_impl->getObjectFilePath(outputPath, source);

        Command *command = m_impl->createCompileCommand(outputPath, source, options);
        DagNode *node = dag->createNode(command);
        
        // TODO: compute dependent headers
        node->createDependency(m_impl->commandFactory->createPathCommand(outputFileRelativePath.parent_path(), PathCommand::Create));

        return CompileOutput{outputFileRelativePath, node};
    }


    std::vector<boost::filesystem::path> CompilerImpl::computeDependencies(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
        return m_impl->computeFileDependencies(source, options);
    }


    boost::filesystem::path CompilerImpl::compileOutputFile(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
        return m_impl->getObjectFilePath(outputPath, source);
    }
}
