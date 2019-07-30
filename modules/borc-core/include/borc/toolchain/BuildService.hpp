
#ifndef __BORC_MODEL_BUILDSERVICE_HPP__
#define __BORC_MODEL_BUILDSERVICE_HPP__

#include <string>
#include <vector>


namespace borc {
	class Compiler;
	class Linker;
	class Package;

	class BuildService {
	private:
		const Compiler *compiler;
		const Linker *linker;

		//! list of extension wildcards that will trigger a source file compilation with the current compiler
		const std::vector<std::string> compilerWildcards = {
			"*.c", "*.cpp", "*.c++", "*.cc", "*.cxx",
		};

	public:
		explicit BuildService(const Compiler *compiler, const Linker *linker);

		void buildProject(const Package *project);

	private:
		bool isFileCompilable(const std::string &file) const;
	};
}

#endif
