
#ifndef __BORC_TOOLCHAIN_ARTIFACTTYPECHECKER_HPP__
#define __BORC_TOOLCHAIN_ARTIFACTTYPECHECKER_HPP__

#include <borc/toolchain/ArtifactChecker.hpp>

#include <borc/model/Artifact.hpp>
#include <set>
#include <initializer_list>

namespace borc {
    class ArtifactTypeChecker : public ArtifactChecker {
	public:
		ArtifactTypeChecker(const std::initializer_list<Artifact::Type> &types);

		virtual ~ArtifactTypeChecker();

		virtual bool check(const Artifact *artifact) const override;

	private:
		std::set<Artifact::Type> types;
	};
}

#endif
