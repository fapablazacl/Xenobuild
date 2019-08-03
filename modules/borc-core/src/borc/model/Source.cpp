
#include <borc/model/Source.hpp>

#include <stdexcept>
#include <boost/filesystem.hpp>

namespace borc {
	Source::Source(const Artifact *artifact, const boost::filesystem::path &filePath, const boost::filesystem::path &relativeFilePath) {
		if (! filePath.is_absolute()) {
			throw std::runtime_error("Source::Source: 'filePath' must be an absolute path.");
		}

		if (! relativeFilePath.is_relative()) {
			throw std::runtime_error("Source::Source: 'relativeFilePath' must be a relative path.");
		}

		this->artifact = artifact;
		this->filePath = filePath;
		this->relativeFilePath = relativeFilePath;
	}

	Source::~Source() {}
}
