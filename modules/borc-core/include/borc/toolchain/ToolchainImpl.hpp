
#ifndef __BORC_CORE_TOOLCHAINIMPL_HPP__
#define __BORC_CORE_TOOLCHAINIMPL_HPP__

#include <set>
#include <boost/filesystem.hpp>

#include "Toolchain.hpp"

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

	class ToolchainImpl : public Toolchain {
	public:
		explicit ToolchainImpl(const std::vector<std::pair<SourceType, const Compiler*>> &compilers, const Linker *linker);

		virtual ~ToolchainImpl();

		virtual const Compiler* selectCompiler(const Source *source) const override;

		virtual const Linker* selectLinker(const Artifact *artifact) const override;

	private:
		std::vector<std::pair<SourceType, const Compiler*>> compilers;
		const Linker *linker = nullptr;
	};
}

#endif
