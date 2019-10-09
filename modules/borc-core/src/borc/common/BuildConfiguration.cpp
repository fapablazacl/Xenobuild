
#include <borc/common/BuildConfiguration.hpp>
#include <tuple>

namespace borc {
    bool BuildConfiguration::operator< (const BuildConfiguration &other) const {
        if (toolchainId >= other.toolchainId) {
            return false;
        }

        if (arch >= other.arch) {
            return false;
        }

        if (version >= other.version) {
            return false;
        }

        if (type >= other.type) {
            return false;
        }

        return false;
    }

    std::string BuildConfiguration::computeIdentifier() const {
        return toolchainId + "-" + static_cast<std::string>(version) + "(" + arch + ")";
    }
}
