
#include <bok/core/package/Source.hpp>

#include <stdexcept>
#include <cassert>
#include <boost/filesystem.hpp>

namespace bok {
    Source::Source(const Module *component, const boost::filesystem::path &filePath, const boost::filesystem::path &relativeFilePath) {
        assert(filePath.is_absolute());
        assert(relativeFilePath.is_relative());

        this->component = component;
        this->filePath = filePath;
        this->relativeFilePath = relativeFilePath;
    }

    Source::~Source() {}

    std::time_t Source::getModifiedTime() const {
        return boost::filesystem::last_write_time(filePath);
    }
}
