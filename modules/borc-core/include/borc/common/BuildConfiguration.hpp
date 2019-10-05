
#ifndef __BORC_COMMON_BUILDCONFIGURATION_HPP__
#define __BORC_COMMON_BUILDCONFIGURATION_HPP__

#include <vector>
#include <utility>
#include <tuple>
#include <borc/model/Version.hpp>

namespace borc {
    struct BuildConfiguration {
        std::string toolchain;
        Version version;

        /*
        constexpr static auto properties = std::make_tuple (
            property(&BuildConfiguration::toolchain, "toolchain"),
            property(&BuildConfiguration::version, "version")
        );
        */
    };
}

#endif
