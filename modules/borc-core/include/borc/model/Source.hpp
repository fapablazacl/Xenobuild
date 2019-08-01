
#ifndef __BORC_MODEL_SOURCE_HPP__
#define __BORC_MODEL_SOURCE_HPP__

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace borc {
	class Artifact;

	class Source {
	public:
		explicit Source(const Artifact *artifact, const boost::filesystem::path &partialFilePath);

		~Source();

		const Artifact* getArtifact() const {
			return artifact;
		}

		boost::filesystem::path getPartialFilePath() const {
			return partialFilePath;
		}

	private:
		boost::filesystem::path partialFilePath;
		const Artifact *artifact = nullptr;
	};
}

#endif
