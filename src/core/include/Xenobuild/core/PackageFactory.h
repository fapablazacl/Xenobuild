
#pragma once 

#include "Package.h"
#include <istream>

namespace Xenobuild {
    class PackageFactory {
    public:
        virtual ~PackageFactory() {}

        virtual Package createMockPackage() = 0;

        virtual Package createPackage(const std::string& sourceDir) = 0;
        
        //! Creates a Package from the given Text-based input stream
        virtual Package createPackage(std::istream &is) = 0;
    };
}
