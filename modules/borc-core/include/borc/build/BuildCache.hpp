
#ifndef __BORC_BUILD_BUILDCACHE_HPP__
#define __BORC_BUILD_BUILDCACHE_HPP__

namespace borc {
    class Artifact;
    class Source;
    class BuildCache {
    public:
        virtual ~BuildCache();

        virtual bool needsRebuild(const Artifact *artifact) = 0;

        virtual bool needsRebuild(const Source *source) = 0;
    };
}

#endif
