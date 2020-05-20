
#include "PackageRegistryFactory.hpp"

#include <boost/filesystem.hpp>
#include <borc/services/PackageService.hpp>
#include "Package.hpp"
#include "PackageRegistry.hpp"

namespace bok {
    std::unique_ptr<PackageRegistry> PackageRegistryFactory::createPackageRegistry(PackageService *packageService, const boost::filesystem::path &packageRegistryPath) const {
        using boost::filesystem::directory_entry;
        using boost::filesystem::directory_iterator;
        using boost::filesystem::is_directory;

        auto packageRegistry = std::make_unique<PackageRegistry>();

        /*
        for (directory_entry &entry : directory_iterator{packageRegistryPath}) {
            if (! is_directory(entry.path())) {
                continue;
            }

            auto package = packageService->createPackage(entry.path(), nullptr);

            packageRegistry->registerPackage(std::move(package));
        }
        */

        return packageRegistry;
    }
}
