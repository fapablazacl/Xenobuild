
#ifndef __BORC_TOOLCHAIN_COMPILER_HPP__
#define __BORC_TOOLCHAIN_COMPILER_HPP__

#include <vector>
#include <iostream>
#include <boost/filesystem/path.hpp>

namespace borc {
	class Dag;
	class DagNode;

	struct CompileOptions;

	struct CompileOutput {
		boost::filesystem::path outputFileRelativePath;
		DagNode *node = nullptr;
	};

	class Source;
	class Compiler {
	public:
		virtual ~Compiler();

		virtual CompileOutput compile(Dag *dag, const boost::filesystem::path &outputPath, const Source *source, const CompileOptions &options) const = 0;
	};
}

#endif
