
#ifndef __BOK_BUILD_BUILDCACHE_HPP__
#define __BOK_BUILD_BUILDCACHE_HPP__

#include <vector>
#include <set>
#include <map>
#include <boost/optional.hpp>

#include <borc/common/Types.hpp>
#include <borc/common/BuildConfiguration.hpp>

namespace bok {
    /**
     * @brief Provides storage to all the build cache metadata for the current borc package.
     * 
     * This metadata is used later for configuring builds
     */
    class BuildCache {
    public:
        virtual ~BuildCache();

        /**
         * @brief Check if the current timestamp in the filesystem is different against the registered one
         */
        virtual bool needsRebuild(const path &path) const = 0;

        virtual void markAsBuilt(const path &path) = 0;
    };
}

#endif
