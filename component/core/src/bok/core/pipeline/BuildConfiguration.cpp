
#include <bok/core/pipeline/BuildConfiguration.hpp>

#include <tuple>

namespace bok {
    bool BuildConfiguration::operator< (const BuildConfiguration &other) const {
        if (toolchainId >= other.toolchainId) {
            return false;
        }

        if (version >= other.version) {
            return false;
        }

        if (arch >= other.arch) {
            return false;
        }

        return true;
    }

    bool BuildConfiguration::operator== (const BuildConfiguration &rhs) const {
        if (toolchainId != rhs.toolchainId) {
            return false;
        }

        if (version != rhs.version) {
            return false;
        }

        if (arch != rhs.arch) {
            return false;
        }

        return true;
    }

    std::string BuildConfiguration::computeIdentifier() const {
        return toolchainId + "-" + static_cast<std::string>(version) + "-" + arch;
    }
}
