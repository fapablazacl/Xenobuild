
#ifndef __BORC_CORE_TOOLCHAINIMPL_HPP__
#define __BORC_CORE_TOOLCHAINIMPL_HPP__

#include <set>
#include <boost/filesystem.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/model/Artifact.hpp>

namespace borc {
	struct SourceType {
		SourceType(const std::initializer_list<std::string> &wildcards);

		bool match(const boost::filesystem::path &filePath) const;

		std::set<std::string> wildcards;
	};

	class ArtifactChecker {
	public:
		virtual ~ArtifactChecker() {}

		virtual bool check(const Artifact *artifact) const = 0;
	};

	class ArtifactTypeChecker : public ArtifactChecker {
	public:
		ArtifactTypeChecker(const Artifact::Type type) {
			this->type = type;
		}

		virtual ~ArtifactTypeChecker() {}

		virtual bool check(const Artifact *artifact) const override {
			return type == artifact->getType();
		}

	private:
		Artifact::Type type;
	};
}

namespace borc {
	class Compiler;
	class Linker;
	class Source;
	class Artifact;

	class ToolchainImpl : public Toolchain {
	public:
		explicit ToolchainImpl(const std::vector<std::pair<SourceType, const Compiler*>> &compilers, const std::vector<std::pair<ArtifactChecker*, const Linker*>> &linkers);

		virtual ~ToolchainImpl();

		virtual const Compiler* selectCompiler(const Source *source) const override;

		virtual const Linker* selectLinker(const Artifact *artifact) const override;

	private:
		std::vector<std::pair<SourceType, const Compiler*>> compilers;
		std::vector<std::pair<ArtifactChecker*, const Linker*>> linkers;
	};
}

#endif
