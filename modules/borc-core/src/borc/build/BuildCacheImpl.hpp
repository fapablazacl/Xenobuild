
#ifndef __BORC_BUILD_BUILDCACHEIMPL_HPP__
#define __BORC_BUILD_BUILDCACHEIMPL_HPP__

#include <borc/build/BuildCache.hpp>

#include <ctime>
#include <map>

namespace borc {
    class BuildCacheImpl : public BuildCache {
    public:
        BuildCacheImpl(const boost::filesystem::path &outputPath);

        virtual ~BuildCacheImpl();

        virtual bool needsRebuild(const boost::filesystem::path &path) const override;

        virtual void markAsBuilt(const boost::filesystem::path &path) override;

        virtual BuildCacheData getData() const override {
            return buildCacheData;
        }

        virtual void addBuildConfiguration(const BuildConfiguration &config) override;

    private:
        //! loads the build configuration from the current output path
        void loadConfigurations();

        void saveConfigurations();

        void loadCache();

        void saveCache();

        std::string getCacheFileName() const;

        std::time_t computeMark(const boost::filesystem::path &path) const;

    private:
        boost::filesystem::path outputPath;
        std::map<boost::filesystem::path, std::time_t> pathTimeMap;
        BuildCacheData buildCacheData;
    };
}

#endif
