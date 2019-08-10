
#ifndef __BORC_MODEL_SOURCE_HPP__
#define __BORC_MODEL_SOURCE_HPP__

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace borc {
	class Artifact;

	class Source {
	public:
		explicit Source(const Artifact *artifact, const boost::filesystem::path &filePath, const boost::filesystem::path &relativeFilePath);

		~Source();

		const Artifact* getArtifact() const {
			return artifact;
		}

		boost::filesystem::path getFilePath() const {
			return filePath;
		}

		boost::filesystem::path getRelativeFilePath() const {
			return relativeFilePath;
		}

		std::time_t getModifiedTime() const;

	private:
		boost::filesystem::path filePath;
		boost::filesystem::path relativeFilePath;
		const Artifact *artifact = nullptr;
	};
}

#endif
