
#ifndef __BORC_BUILD_BUILDCACHEIMPL_HPP__
#define __BORC_BUILD_BUILDCACHEIMPL_HPP__

#include <borc/build/BuildCache.hpp>

namespace borc {
    class BuildCacheImpl : public BuildCache {
    public:
        BuildCacheImpl();

        virtual ~BuildCacheImpl();

        virtual bool needsRebuild(const Artifact *artifact) override;

        virtual bool needsRebuild(const Source *source) override;
    };
}

#endif
