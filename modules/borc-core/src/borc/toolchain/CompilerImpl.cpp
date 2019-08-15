
#include "CompilerImpl.hpp"

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <borc/model/Source.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/Command.hpp>
#include <borc/model/CommandFactory.hpp>
#include <borc/toolchain/CompileOptions.hpp>
#include <borc/utility/Dag.hpp>
#include <borc/utility/DagNode.hpp>


namespace borc {
	CompilerImpl::CompilerImpl(CommandFactory *commandFactory, const std::string &commandPath, const CompilerSwitches &switches, const CompilerConfiguration &configuration) {
		this->commandFactory = commandFactory;
		this->commandPath = commandPath;
		this->switches = switches;
		this->configuration = configuration;
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

		std::vector<std::string> commandOptions = {
			switches.zeroOptimization,
			switches.includeDebug,
			switches.compile,
			"\"" + sourceFilePath.string() + "\"",
			switches.objectFileOutput + "\"" + objectFilePath.string() + "\"",
		};

		// compute system include directories
		for (const std::string &path : configuration.systemIncludePaths) {
			const std::string includeOption = switches.includePath + path;

			commandOptions.push_back(includeOption);
		}

		// compute additional include directories
		for (const std::string &path : options.includePaths) {
			const std::string includeOption = switches.includePath + path;

			commandOptions.push_back(includeOption);
		}

		// add additional compiler options
		commandOptions.insert(commandOptions.end(), std::begin(configuration.flags), std::end(configuration.flags));

		return commandFactory->createCommand(commandPath, commandOptions);
	}

	CompileOutput CompilerImpl::compile(Dag *dag, const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
		// ******************
		this->computeFileDependencies(outputPath, source, options);

		const boost::filesystem::path outputFileRelativePath = this->getObjectFilePath(outputPath, source);

		Command *command = this->createCompileCommand(outputPath, source, options);
		DagNode *node = dag->createNode(command);
		
		// TODO: compute dependent headers
		node->createDependency(commandFactory->createPathCommand(outputFileRelativePath.parent_path(), PathCommand::Create));

		return CompileOutput{outputFileRelativePath, node};
	}

	std::vector<boost::filesystem::path> CompilerImpl::computeFileDependencies(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
		const auto sourceFilePath = source->getFilePath();
		const auto objectFilePath = this->getObjectFilePath(outputPath, source);

		std::vector<std::string> commandOptions = {
			switches.generateDependencies,
			"\"" + sourceFilePath.string() + "\""
		};

		// compute system include directories
		for (const std::string &path : configuration.systemIncludePaths) {
			const std::string includeOption = switches.includePath + path;

			commandOptions.push_back(includeOption);
		}

		// compute additional include directories
		for (const std::string &path : options.includePaths) {
			const std::string includeOption = switches.includePath + path;

			commandOptions.push_back(includeOption);
		}

		// add additional compiler options
		commandOptions.insert(commandOptions.end(), std::begin(configuration.flags), std::end(configuration.flags));

		std::string opt;
		for (const std::string &option : commandOptions) {
			opt += " " + option;
		}

		// log output
		std::cout << "**************************" << std::endl;
		std::cout << commandPath;
		for (const std::string &option : commandOptions) {
			std::cout << " " << option;
		}
		std::cout << std::endl << "**************************" << std::endl;

		boost::filesystem::path compilerPath = boost::process::search_path(commandPath);
		boost::process::ipstream pipeStream;
        // boost::process::child childProcess {compilerPath, boost::process::args(commandOptions), boost::process::std_out > pipeStream};
		boost::process::child childProcess {compilerPath, opt, boost::process::std_out > pipeStream};

        std::string line;
        std::vector<std::string> specs;

        while (pipeStream && std::getline(pipeStream, line) && !line.empty()) {
            specs.push_back(line);
        }

        childProcess.wait();

		// log output
		std::cout << "**************************" << std::endl;
		for (const std::string &line : specs) {
			std::cout << line << std::endl;
		}
		std::cout << "**************************" << std::endl;

		std::vector<boost::filesystem::path> dependencies;

		return dependencies;
	}
}
