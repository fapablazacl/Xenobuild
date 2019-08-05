
#ifndef __BORC_TOOLCHAIN_LINKER_HPP__
#define __BORC_TOOLCHAIN_LINKER_HPP__

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace borc {
	class Command;
	class Package;
	class Artifact;

	struct LinkOutput {
		boost::filesystem::path outputArtifactRelativePath;
		Command *command = nullptr;
	};

	class Linker {
	public:
		virtual ~Linker();

		virtual LinkOutput link(const boost::filesystem::path &outputPath, const Package *package, const Artifact *artifact, const std::vector<boost::filesystem::path> &objectFiles) const = 0;
	};
}

#endif
