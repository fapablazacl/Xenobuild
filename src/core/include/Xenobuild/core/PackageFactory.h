
#pragma once 

#include "Package.h"

namespace Xenobuild {
    class PackageFactory {
    public:
        virtual ~PackageFactory() {}

        virtual Package createMockPackage() = 0;

        virtual Package createPackage(const std::string& sourceDir) = 0;
    };
}
