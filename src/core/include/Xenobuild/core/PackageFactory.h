
#pragma once 

#include <istream>

namespace Xenobuild {
    struct Package;
    
    class PackageFactory {
    public:
        virtual ~PackageFactory() {}

        virtual Package createMockPackage() = 0;

        virtual Package createPackage(const std::string& sourceDir) = 0;
        
        //! Creates a Package from the given Text-based input stream
        virtual Package createPackage(std::istream &is) = 0;
    };
}
