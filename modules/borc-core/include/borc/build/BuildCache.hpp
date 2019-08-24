
#ifndef __BORC_BUILD_BUILDCACHE_HPP__
#define __BORC_BUILD_BUILDCACHE_HPP__

#include <boost/filesystem/path.hpp>

namespace borc {
    class BuildCache {
    public:
        virtual ~BuildCache();

        /**
         * @brief Check if the current timestamp in the filesystem is different against the registered one
         */
        virtual bool needsRebuild(const boost::filesystem::path &path) = 0;
    };
}

#endif
