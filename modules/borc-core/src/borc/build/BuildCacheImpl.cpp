
#include <borc/build/BuildCacheImpl.hpp>

namespace borc {
    BuildCacheImpl::BuildCacheImpl() {}

    BuildCacheImpl::~BuildCacheImpl() {}

    bool BuildCacheImpl::needsRebuild(const Artifact *artifact) {
        return true;
    }

    bool BuildCacheImpl::needsRebuild(const Source *source) {
        return true;
    }   
}
