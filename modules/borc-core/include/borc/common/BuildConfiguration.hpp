
#ifndef __BORC_COMMON_BUILDCONFIGURATION_HPP__
#define __BORC_COMMON_BUILDCONFIGURATION_HPP__

#include <string>
#include <vector>
#include <borc/model/Version.hpp>

namespace borc {
    /**
     * @brief Abstracts the concrete compiler and linker flags used to build object files and binaries.
     */
    struct BuildType {
        std::string type;

        BuildType() {}

        explicit BuildType(const std::string &type) {
            this->type = type;
        }

        explicit operator std::string() const {
            return type;
        }

        typedef std::string DefaultType;
    };

    /**
     * @brief A build configuration
     */
    struct BuildConfiguration {
        // The following types are considered a composed identifier for the operator< implementation
        
        //! the build toolchainId used in the configuration
        std::string toolchainId;

        //! the version
        Version version;

        //! target architecture 
        std::string arch;

        //! build types associated to this build configuration
        std::vector<BuildType> buildTypes;

        bool operator< (const BuildConfiguration &other) const;

        bool operator== (const BuildConfiguration &rhs) const;

        bool operator!= (const BuildConfiguration &rhs) const {
            return !(*this == rhs);
        }

        /**
         * @brief Generates a std::string with the format: 'toolchainId-version-arch'
         */ 
        std::string computeIdentifier() const;

        typedef void DefaultType;
    };
}

#endif
