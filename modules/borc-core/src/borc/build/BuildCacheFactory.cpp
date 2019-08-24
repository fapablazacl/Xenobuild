
#include <borc/build/BuildCacheFactory.hpp>

#include "BuildCacheImpl.hpp"

namespace borc {
    BuildCacheFactory::BuildCacheFactory() {}

    BuildCacheFactory::~BuildCacheFactory() {}

    BuildCache* BuildCacheFactory::createBuildCache(const boost::filesystem::path &cachePath) {
        auto cache = new BuildCacheImpl(cachePath);

        cacheStorage.emplace_back(cache);

        return cache;
    }
}
