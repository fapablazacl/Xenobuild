
#ifndef __BORC_BUILD_BUILDCACHE_HPP__
#define __BORC_BUILD_BUILDCACHE_HPP__

#include <vector>
#include <set>
#include <map>
#include <boost/optional.hpp>
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

        //! The currently selected build configuration. It used by default by all the subcommands
        boost::optional<BuildConfiguration> currentBuildConfiguration;
    };

    /**
     * @brief Provides storage to all the build cache metadata for the current borc package.
     * 
     * This metadata is used later for configuring builds
     */
    class BuildCache {
    public:
        virtual ~BuildCache();

        /**
         * @brief Retrieves the currently associated build-cached metadata
         */
        virtual BuildCacheData getData() const = 0;

        /**
         * @brief Check if the current timestamp in the filesystem is different against the registered one
         */
        virtual bool needsRebuild(const boost::filesystem::path &path) const = 0;

        virtual void markAsBuilt(const boost::filesystem::path &path) = 0;
    };
}

#endif
