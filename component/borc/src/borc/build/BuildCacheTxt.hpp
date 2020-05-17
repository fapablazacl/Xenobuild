
#ifndef __BORC_BUILD_BUILDCACHEIMPL_HPP__
#define __BORC_BUILD_BUILDCACHEIMPL_HPP__

#include "BuildCache.hpp"

#include <ctime>
#include <map>

namespace bok {
    class BuildCacheTxt : public BuildCache {
    public:
        explicit BuildCacheTxt(const path &outputPath, const path &prefixPath);

        virtual ~BuildCacheTxt();

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
