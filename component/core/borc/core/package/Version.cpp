
#include "Version.hpp"

#include <tuple>

namespace borc {
    Version::Version() {}

    Version::Version(const int major_, const int minor_, const int revision_) 
        : major(major_), minor(minor_), revision(revision_) {}

    bool Version::operator== (const Version &rhs) const {
        return std::tuple(major, minor, revision) == std::tuple(rhs.major, rhs.minor, rhs.revision);
    }
}
