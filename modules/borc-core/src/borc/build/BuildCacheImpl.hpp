
#ifndef __BORC_BUILD_BUILDCACHEIMPL_HPP__
#define __BORC_BUILD_BUILDCACHEIMPL_HPP__

#include <borc/build/BuildCache.hpp>

#include <ctime>
#include <map>

namespace borc {
    class BuildCacheImpl : public BuildCache {
    public:
        BuildCacheImpl();

        virtual ~BuildCacheImpl();

        virtual bool needsRebuild(const boost::filesystem::path &filePath) const override;

        virtual void putCache(const boost::filesystem::path &filePath) override;

    private:
        std::map<boost::filesystem::path, std::time_t> fileModifiedMap;
    };
}

#endif
