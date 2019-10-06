
#ifndef __BORC_BUILD_BUILDCACHE_HPP__
#define __BORC_BUILD_BUILDCACHE_HPP__

#include <vector>
#include <set>
#include <map>
#include <boost/filesystem/path.hpp>
#include <borc/common/BuildConfiguration.hpp>

namespace borc {
    /**
     * @brief Build cache data
     */
    struct BuildCacheData {
        //! Build-cached Source File typedef. Can be evolved with more metadata in the future.
        typedef boost::filesystem::path Source;

        //! Build-cached Set of sources. Can be evolved later with more metadata in the future.
        typedef std::set<Source> SourceSet;

        //! Map from a build configuration to a set of sources, that where used in the last/current build
        std::map<BuildConfiguration, SourceSet> sourceSetMap;
    };

    /**
     * @brief 
     */
    class BuildCache {
    public:
        virtual ~BuildCache();

        virtual std::set<BuildConfiguration> getBuildConfigurationSet() const = 0;

        /**
         * @brief Check if the current timestamp in the filesystem is different against the registered one
         */
        virtual bool needsRebuild(const boost::filesystem::path &path) const = 0;

        virtual void markAsBuilt(const boost::filesystem::path &path) = 0;
    };
}

#endif
