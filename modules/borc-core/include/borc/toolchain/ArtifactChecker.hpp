
#ifndef __BORC_TOOLCHAIN_ARTIFACTCHECKER_HPP__
#define __BORC_TOOLCHAIN_ARTIFACTCHECKER_HPP__

namespace borc {
    class Artifact;
    class ArtifactChecker {
	public:
		virtual ~ArtifactChecker();

		virtual bool check(const Artifact *artifact) const = 0;
	};
}

#endif
