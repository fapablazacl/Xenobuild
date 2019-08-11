
#ifndef __BORC_BUILD_BUILDCACHEFACTORY_HPP__
#define __BORC_BUILD_BUILDCACHEFACTORY_HPP__

#include <boost/filesystem/path.hpp>
#include <memory>
#include <vector>

namespace borc {
    class BuildCache;
    class BuildCacheFactory {
    public:
        BuildCacheFactory();
        
        ~BuildCacheFactory();

        BuildCache* createBuildCache(const boost::filesystem::path &cacheFilePath);

    private:
        std::vector<std::unique_ptr<BuildCache>> cacheStorage;
    };
}

#endif