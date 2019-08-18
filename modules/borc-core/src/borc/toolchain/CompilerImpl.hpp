
#ifndef __BORC_TOOLCHAIN_COMPILERIMPL_HPP__
#define __BORC_TOOLCHAIN_COMPILERIMPL_HPP__

#include <borc/toolchain/Compiler.hpp>

#include <vector>
#include <iostream>
#include <boost/filesystem/path.hpp>

namespace borc {
	class DagNode;
	class Package;
	class Artifact;
	class Command;
	class CommandFactory;

	struct CompilerSwitches {
		std::string compile;
		std::string objectFileOutput;
		std::string includeDebug;
		std::string zeroOptimization;
		std::string includePath;
		std::string generateDependencies;

		CompilerSwitches() {}
	};

	//! Compiler default configuration
	struct CompilerConfiguration {
		std::vector<std::string> flags;
		std::vector<std::string> systemIncludePaths;
	};

	class Source;
	class Dag;
	class DagNode;
	class CompilerImpl : public Compiler {
	public:
		explicit CompilerImpl(
			CommandFactory *commandFactory, 
			const std::string &commandPath, 
			const CompilerSwitches &switches, 
			const CompilerConfiguration &configuration
		);

		virtual CompileOutput compile(Dag *dag, const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const override;

		virtual std::vector<boost::filesystem::path> computeDependencies(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const override;

		virtual boost::filesystem::path compiteOutputFile(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const override;

	private:
		Command* createCompileCommand(const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const;

		boost::filesystem::path getObjectFilePath(const boost::filesystem::path &outputPath, const Source *source) const;

		std::vector<boost::filesystem::path> computeFileDependencies(const Source *source, const CompileOptions &options) const;

	private:
		CommandFactory *commandFactory = nullptr;
		std::string commandPath;
		CompilerSwitches switches;
		CompilerConfiguration configuration;
	};
}

#endif
