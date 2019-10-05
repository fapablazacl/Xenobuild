
#include <borc/common/EntityLoaderFactory.hpp>

#include <memory>
#include <boost/filesystem.hpp>

#include <borc/common/JSONEntityLoader.hpp>

namespace borc {
    EntityLoaderFactory::~EntityLoaderFactory() {}

    std::unique_ptr<EntityLoader> EntityLoaderFactory::createLoader(const boost::filesystem::path &packagePath, FileService &fileService) {
        if (boost::filesystem::exists(packagePath / "borc-package.json")) {
            return std::make_unique<JSONEntityLoader>(packagePath, fileService);
        }

        throw std::runtime_error("Path " + packagePath.string() + " doesn't contain a valid package file");
    }
}
