
#ifndef __BORC_CORE_TOOLCHAINIMPL_HPP__
#define __BORC_CORE_TOOLCHAINIMPL_HPP__

#include <set>
#include <boost/filesystem.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/model/Artifact.hpp>

namespace borc {
	class Compiler;
	class Linker;
	class Source;
	class Artifact;
	class SourceChecker;
	class ArtifactChecker;

	class ToolchainImpl : public Toolchain {
	public:
		explicit ToolchainImpl(const std::vector<std::pair<SourceChecker*, const Compiler*>> &compilers, const std::vector<std::pair<ArtifactChecker*, const Linker*>> &linkers);

		virtual ~ToolchainImpl();

		virtual const Compiler* selectCompiler(const Source *source) const override;

		virtual const Linker* selectLinker(const Artifact *artifact) const override;

	private:
		std::vector<std::pair<SourceChecker*, const Compiler*>> compilers;
		std::vector<std::pair<ArtifactChecker*, const Linker*>> linkers;
	};
}

#endif
