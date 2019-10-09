
#ifndef __BORC_COMMON_BUILDCONFIGURATION_HPP__
#define __BORC_COMMON_BUILDCONFIGURATION_HPP__

#include <string>
#include <borc/model/Version.hpp>

namespace borc {
    /**
     * @brief The
     */
    struct BuildConfiguration {
        //! the build toolchainId used in the configuration
        std::string toolchainId;

        //! target architecture 
        std::string arch;

        //! the version
        Version version;

        //! build type
        std::string type;

        bool operator< (const BuildConfiguration &other) const;

        std::string computeIdentifier() const;
    };
}

#endif
