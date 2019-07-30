
#ifndef __BORC_CORE_TOOLCHAIN_HPP__
#define __BORC_CORE_TOOLCHAIN_HPP__

#include <set>
#include <boost/filesystem.hpp>

namespace borc {
	struct SourceType {
		SourceType(const std::initializer_list<std::string> &wildcards);

		bool match(const boost::filesystem::path &filePath) const;

		std::set<std::string> wildcards;
	};
}

namespace borc {
	class Compiler;
	class Linker;
	class Source;
	class Artifact;

	class Toolchain {
	public:
		explicit Toolchain(const std::vector<std::pair<SourceType, const Compiler*>> &compilers, const Linker *linker);

		~Toolchain();

		const Compiler* selectCompiler(const Source *source) const;

		const Linker* selectLinker(const Artifact *artifact) const;

	private:
		std::vector<std::pair<SourceType, const Compiler*>> compilers;
		const Linker *linker = nullptr;
	};
}

#endif
