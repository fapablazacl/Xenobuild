
#ifndef __BORC_BUILD_BUILDCACHE_HPP__
#define __BORC_BUILD_BUILDCACHE_HPP__

#include <boost/filesystem/path.hpp>

namespace borc {
    class BuildCache {
    public:
        virtual ~BuildCache();

        virtual bool needsRebuild(const boost::filesystem::path &filePath) const = 0;

        virtual void putCache(const boost::filesystem::path &filePath) = 0;
    };
}

#endif
