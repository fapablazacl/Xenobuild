
#ifndef __BORC_MODEL_BUILDCOMPILER_HPP__
#define __BORC_MODEL_BUILDCOMPILER_HPP__

#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>
#include "Predef.h"

namespace borc {
	class Project;
	class Module;
	class Command;
	class CommandFactory;

	struct CompilerSwitches {
		std::string compile;
		std::string objectFileOutput;
		std::string includeDebug;
		std::string zeroOptimization;
		std::string includePath;

		CompilerSwitches() {}
	};

	//! Compiler default configuration
	struct CompilerConfiguration {
		std::vector<std::string> flags;
		std::vector<std::string> systemIncludePaths;
	};

	struct CompileOptions {
		std::vector<std::string> includePaths;
	};

	class Source;
	class Compiler {
	public:
		explicit Compiler(
			CommandFactory *commandFactory, 
			const std::string &commandPath, 
			const CompilerSwitches &switches, 
			const CompilerConfiguration &configuration
		);

		[[deprecated]]
		std::string compile(const Project *project, const Module *module, const std::string &file, const CompileOptions &options) const;

		Command* createCompileCommand(const Source *source, const CompileOptions &options) const;

		boost::filesystem::path getObjectFilePath(const Source *source) const;

	private:
		CommandFactory *commandFactory = nullptr;
		std::string commandPath;
		CompilerSwitches switches;
		CompilerConfiguration configuration;
	};
}

#endif
