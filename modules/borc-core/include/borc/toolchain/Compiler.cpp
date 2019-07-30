
#include "Compiler.hpp"
#include "Common.hpp"
#include "Command.hpp"
#include "CommandFactory.hpp"

#include "Source.hpp"
#include "Project.hpp"
#include "Module.hpp"

namespace borc {
	Compiler::Compiler(CommandFactory *commandFactory, const std::string &commandPath, const CompilerSwitches &switches, const CompilerConfiguration &configuration) {
		this->commandFactory = commandFactory;
		this->commandPath = commandPath;
		this->switches = switches;
		this->configuration = configuration;
	}

	std::string Compiler::compile(const Project *project, const Module *module, const std::string &file, const CompileOptions &options) const {
		const boost::filesystem::path sourceFilePath = boost::filesystem::canonical(module->getPath() / boost::filesystem::path(file));
		const boost::filesystem::path objectFilePath = boost::filesystem::canonical(module->getOutputPath()) / boost::filesystem::path(file + ".obj");

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

		return objectFilePath.string();
	}

	boost::filesystem::path Compiler::getObjectFilePath(const Source *source) const {
		const auto objectBaseFilePath = source->getPartialFilePath().string() + ".obj";
		const auto outputPath = source->getParentModule()->getOutputPath();
		const auto objectFilePath = boost::filesystem::canonical(outputPath) / boost::filesystem::path(objectBaseFilePath);

		return objectFilePath;
	}

	Command* Compiler::createCompileCommand(const Source *source, const CompileOptions &options) const {
		const auto sourceFilePath = source->getFilePath();
		const auto objectFilePath = this->getObjectFilePath(source);

		std::cout << "    " << source->getPartialFilePath() << " ..." << std::endl;

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

		return command;
	}
}
