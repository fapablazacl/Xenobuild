
#ifndef __BORC_MODEL_PACKAGECONTEXT_HPP__
#define __BORC_MODEL_PACKAGECONTEXT_HPP__

#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <boost/filesystem/path.hpp>

namespace borc {
    class Package;
    class Context {
    public:
        explicit Context(const boost::filesystem::path &outputPath, Package *package, std::vector<std::unique_ptr<Package>> packages);

        ~Context();

        /**
         * @brief Get the currently loaded packages 
         */
        std::vector<Package*> getPackages() const;

        /**
         * @brief Check if the current timestamp in the filesystem is different against the registered one
         */
        Package* getMainPackage() const;

        /**
         * @brief Check if the current timestamp in the filesystem is different against the registered one
         */
        bool isModified(const boost::filesystem::path &path);

    private:
        boost::filesystem::path outputPath;
        Package *package = nullptr;
        std::vector<std::unique_ptr<Package>> packages;
        std::map<boost::filesystem::path, std::time_t> pathTimeMap;
    };
}

#endif
