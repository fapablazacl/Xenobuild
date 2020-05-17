
#ifndef __BORC_SERVICES_PACKAGESERVICE_HPP__
#define __BORC_SERVICES_PACKAGESERVICE_HPP__

#include <memory>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Package;
    class PackageRegistry;

    class PackageService {
    public:
        virtual ~PackageService();

        virtual std::unique_ptr<Package> createPackage(const boost::filesystem::path &packageFilePath, const PackageRegistry *packageRegistry) const = 0;
    }; 
}

#endif
