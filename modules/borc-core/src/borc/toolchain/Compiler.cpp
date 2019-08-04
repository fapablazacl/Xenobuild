
#include <borc/toolchain/Compiler.hpp>

#include <boost/filesystem.hpp>
#include <borc/model/Source.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/Command.hpp>
#include <borc/model/CommandFactory.hpp>
#include <borc/utility/Dag.hpp>
#include <borc/utility/DagNode.hpp>

namespace borc {
	Compiler::Compiler(CommandFactory *commandFactory, const std::string &commandPath, const CompilerSwitches &switches, const CompilerConfiguration &configuration) {
		this->commandFactory = commandFactory;
		this->commandPath = commandPath;
		this->switches = switches;
		this->configuration = configuration;
	}

	std::string Compiler::compile(const Package *package, const Artifact *artifact, const std::string &file, const CompileOptions &options) const {
		const auto sourceFilePath = boost::filesystem::canonical(artifact->getPath() / boost::filesystem::path(file));

		// TODO: Use a valid Outputpath
		const auto objectFilePath = boost::filesystem::canonical(artifact->getPath()) / boost::filesystem::path(file + ".obj");

		/*
		std::cout << "    " << file << " ..." << std::endl;

		Command *command = commandFactory->createCommand(
			commandPath, {
				switches.zeroOptimization,
				switches.includeDebug,
				switches.compile,
				"\"" + sourceFilePath.string() + "\"",
				switches.objectFileOutput + "\"" + objectFilePath.string() + "\"",
			}
		);

		// compute system include directories
		for (const std::string &path : configuration.systemIncludePaths) {
			const std::string includeOption = switches.includePath + path;
			command->addOption(includeOption);
		}

		// compute additional include directories
		for (const std::string &path : options.includePaths) {
			const std::string includeOption = switches.includePath + path;

			command->addOption(includeOption);
		}

		// add additional compiler options
		command->addOptionRange(std::begin(configuration.flags), std::end(configuration.flags));

		command->execute();
		*/

		return objectFilePath.string();
	}

	boost::filesystem::path Compiler::getObjectFilePath(const boost::filesystem::path &outputPath, const Source *source) const {
		const boost::filesystem::path objectFileName = source->getFilePath().filename().string() + ".obj";
		const boost::filesystem::path objectFileParentPath 
			= outputPath 
			// / source->getArtifact()->getPath()
			/ source->getRelativeFilePath().parent_path();

		const boost::filesystem::path objectFilePath = objectFileParentPath / objectFileName;
		
		return objectFilePath;
	}

	Command* Compiler::createCompileCommand(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
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

	CompileOutput Compiler::compile(Dag *dag, const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const {
		Command *command = this->createCompileCommand(outputPath, source, options);
		DagNode *node = dag->createNode(command);
		boost::filesystem::path outputFileRelativePath = this->getObjectFilePath(outputPath, source);

		// TODO: compute dependent headers
		node->previous.push_back(dag->createNode(commandFactory->createPathCommand(outputFileRelativePath.parent_path(), PathCommand::Create)));

		return CompileOutput{outputFileRelativePath, node};
	}
}
