
#include <borc/model/Source.hpp>

namespace borc {
	Source::Source(const Artifact *artifact, const boost::filesystem::path &partialFilePath) {
		this->artifact = artifact;
		this->partialFilePath = partialFilePath;
	}

	Source::~Source() {}
}
