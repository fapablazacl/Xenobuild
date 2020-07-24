
#ifndef __BOK_BUILD_BUILDCACHEIMPL_HPP__
#define __BOK_BUILD_BUILDCACHEIMPL_HPP__

#include "BuildCache.hpp"

#include <ctime>
#include <map>

namespace bok {
    class BuildCacheFileSystem : public BuildCache {
    public:
        explicit BuildCacheFileSystem(const path &outputPath, const path &prefixPath);

        virtual ~BuildCacheFileSystem();

        virtual bool needsRebuild(const path &path) const override;

        virtual void markAsBuilt(const path &path) override;

    private:
        void loadCache();

        void saveCache();

        std::string getCacheFileName() const;

        std::time_t computeMark(const path &path) const;

    private:
        path outputPath;
        path prefixPath;
        std::map<path, std::time_t> pathTimeMap;
    };
}

#endif
