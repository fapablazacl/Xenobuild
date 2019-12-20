
#ifndef __BORC_BUILD_BUILDCACHEIMPL_HPP__
#define __BORC_BUILD_BUILDCACHEIMPL_HPP__

#include "BuildCache.hpp"

#include <ctime>
#include <map>

namespace borc {
    class BuildCacheImpl : public BuildCache {
    public:
        BuildCacheImpl(const boost::filesystem::path &outputPath);

        virtual ~BuildCacheImpl();

        virtual bool needsRebuild(const boost::filesystem::path &path) const override;

        virtual void markAsBuilt(const boost::filesystem::path &path) override;

    private:
        void loadCache();

        void saveCache();

        std::string getCacheFileName() const;

        std::time_t computeMark(const boost::filesystem::path &path) const;

    private:
        boost::filesystem::path outputPath;
        std::map<boost::filesystem::path, std::time_t> pathTimeMap;
    };
}

#endif
