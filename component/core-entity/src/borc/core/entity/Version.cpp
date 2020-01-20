
#include "Version.hpp"

#include <tuple>

namespace borc::core::entity {
    bool Version::operator== (const Version &rhs) const {
        return std::tuple(major, minor, revision) == std::tuple(rhs.major, rhs.minor, rhs.revision);
    }
}
