
#include "CompilerImpl.hpp"

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <borc/model/Source.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/Command.hpp>
#include <borc/model/CommandFactory.hpp>
#include <borc/toolchain/CompileOptions.hpp>
#include <borc/utility/Dag.hpp>
#include <borc/utility/DagNode.hpp>


namespace borc {
    CompilerImpl::CompilerImpl(CommandFactory *commandFactory, const std::string &commandPath, const CompilerImpl::Switches &switches, const std::vector<CompilerImpl::BuildRule> &buildRules) {
        this->commandFactory = commandFactory;
        this->commandPath = commandPath;
        this->switches = switches;
        this->buildRules = buildRules;
    }


    bool CompilerImpl::isSourceLinkable(const Source *source) const {
        return true;
    }


    boost::filesystem::path CompilerImpl::getObjectFilePath(const boost::filesystem::path &outputPath, const Source *source) const {
        const boost::filesystem::path objectFileName = source->getFilePath().filename().string() + ".obj";
        const boost::filesystem::path objectFileParentPath 
            = outputPath 
            / source->getRelativeFilePath().parent_path();

        const boost::filesystem::path objectFilePath = objectFileParentPath / objectFileName;
        
        return objectFilePath;
    }


    Command* CompilerImpl::createCompileCommand(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
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

        return commandFactory->createCommand(commandPath, commandOptions);
    }


    CompileOutput CompilerImpl::compile(Dag *dag, const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
        // ******************
        this->computeFileDependencies(source, options);

        const boost::filesystem::path outputFileRelativePath = this->getObjectFilePath(outputPath, source);

        Command *command = this->createCompileCommand(outputPath, source, options);
        DagNode *node = dag->createNode(command);
        
        // TODO: compute dependent headers
        node->createDependency(commandFactory->createPathCommand(outputFileRelativePath.parent_path(), PathCommand::Create));

        return CompileOutput{outputFileRelativePath, node};
    }


    std::vector<boost::filesystem::path> CompilerImpl::computeFileDependencies(const Source *source, const CompileOptions &options) const {
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


    std::vector<boost::filesystem::path> CompilerImpl::computeDependencies(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
        return this->computeFileDependencies(source, options);
    }


    boost::filesystem::path CompilerImpl::compileOutputFile(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
        return this->getObjectFilePath(outputPath, source);
    }
}
