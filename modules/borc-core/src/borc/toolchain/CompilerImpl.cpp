
#include "CompilerImpl.hpp"

#include <boost/filesystem.hpp>
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
		const boost::filesystem::path outputFileRelativePath = this->getObjectFilePath(outputPath, source);

		Command *command = this->createCompileCommand(outputPath, source, options);
		DagNode *node = dag->createNode(command);
		
		// TODO: compute dependent headers
		node->createDependency(commandFactory->createPathCommand(outputFileRelativePath.parent_path(), PathCommand::Create));

		return CompileOutput{outputFileRelativePath, node};
	}
}