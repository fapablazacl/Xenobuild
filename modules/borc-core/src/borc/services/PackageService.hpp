
#ifndef __BORC_SERVICES_PACKAGESERVICE_HPP__
#define __BORC_SERVICES_PACKAGESERVICE_HPP__

#include <memory>
#include <boost/filesystem/path.hpp>

namespace borc {
    class Package;

    class PackageService {
    public:
        std::unique_ptr<Package> createPackage(const boost::filesystem::path &packageFilePath) const;
    }; 
}

#endif
