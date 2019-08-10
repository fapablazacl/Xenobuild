
#include "BuildCacheImpl.hpp"

#include <boost/filesystem.hpp>

namespace borc {
    BuildCacheImpl::BuildCacheImpl() {}

    BuildCacheImpl::~BuildCacheImpl() {}

    bool BuildCacheImpl::needsRebuild(const boost::filesystem::path &filePath) const {
        if (auto it=fileModifiedMap.find(filePath); it != fileModifiedMap.end()) {
            return boost::filesystem::last_write_time(filePath) != it->second;
        }

        return true;
    }

    void BuildCacheImpl::putCache(const boost::filesystem::path &filePath) {
        fileModifiedMap[filePath] = boost::filesystem::last_write_time(filePath);
    }
}
