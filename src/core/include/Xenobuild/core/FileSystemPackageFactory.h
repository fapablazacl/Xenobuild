
#pragma once 

#include "PackageFactory.h"

namespace Xenobuild {
    class FileSystemPackageFactory : public PackageFactory {
    public:
        Package createMockPackage() override;

        Package createPackage(const std::string& sourceDir) override;
    };
}
