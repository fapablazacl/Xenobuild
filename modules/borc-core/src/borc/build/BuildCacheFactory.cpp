
#include <borc/build/BuildCacheFactory.hpp>

#include "BuildCacheImpl.hpp"

namespace borc {
    class NullBuildCache : public BuildCache {
    public:
        virtual bool needsRebuild(const boost::filesystem::path &path) const override {
            return true;
        }

        virtual void markAsBuilt(const boost::filesystem::path &path) override {}        
    };

    BuildCacheFactory::BuildCacheFactory() {}

    BuildCacheFactory::~BuildCacheFactory() {}

    BuildCache* BuildCacheFactory::createBuildCache(const boost::filesystem::path &cachePath) {
        auto cache = new BuildCacheImpl(cachePath);

        cacheStorage.emplace_back(cache);

        return cache;
    }

    BuildCache* BuildCacheFactory::createNullBuildCache() {
        auto cache = new NullBuildCache();

        cacheStorage.emplace_back(cache);

        return cache;
    }
}
