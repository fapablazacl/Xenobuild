
#ifndef __BOK_SERVICES_PACKAGESERVICE_HPP__
#define __BOK_SERVICES_PACKAGESERVICE_HPP__

#include <memory>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Package;
    class PackageRegistry;

    class PackageFactory {
    public:
        virtual ~PackageFactory();

        virtual std::unique_ptr<Package> createPackage(const boost::filesystem::path &packageFilePath, const PackageRegistry *packageRegistry) const = 0;
    }; 
}

#endif
