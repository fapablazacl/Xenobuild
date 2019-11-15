
#ifndef __BORC_BUILD_CONFIGURATIONSERVICE_HPP__
#define __BORC_BUILD_CONFIGURATIONSERVICE_HPP__

#include <vector>
#include <set>
#include <map>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>
#include <borc/common/BuildConfiguration.hpp>

namespace borc {
    /**
     * @brief Accesible representation of a package complete build configuration
     */
    struct ConfigurationData {
        //! Build-cached Source File typedef. Can be evolved with more metadata in the future.
        typedef boost::filesystem::path Source;

        //! Build-cached Set of sources. Can be evolved later with more metadata in the future.
        typedef std::set<Source> SourceSet;

        // Map from a build configuration to a set of sources, that where used in the last/current build
        // std::map<BuildConfiguration, SourceSet> sourceSetMap;

        std::set<BuildConfiguration> buildConfigurations;

        //! The currently selected build configuration. It used by default by all the subcommands
        boost::optional<BuildConfiguration> currentBuildConfiguration;
    };

    class BuildCache;

    /**
     * @brief Provides storage to all the build cache metadata for the current borc package.
     * 
     * This metadata is used later for configuring builds
     */
    class ConfigurationService {
    public:
        ConfigurationService(const boost::filesystem::path &outputPath);

        /**
         * @brief Retrieves the currently associated build-cached metadata
         */
        ConfigurationData getData() const;

        /**
         * @brief Creates the corresponding build cache of a certain build configuration
         */
        std::unique_ptr<BuildCache> createBuildCache(const BuildConfiguration &config);

        /**
         * @brief Adds a new build configuration to the build cache.
         * 
         * This call will mutate the current root build cache to add this new configuration.
         */
        void addBuildConfiguration(const BuildConfiguration &config);

        /**
         * @brief Saves all configuration information in disk
         */
        void saveAllBuildConfigurations();
    
    private:
        //! loads the build configuration from the current output path
        void loadConfigurations();

        void saveConfigurations();

    private:
        boost::filesystem::path outputPath;
        ConfigurationData configurationData;
    };
}

#endif
