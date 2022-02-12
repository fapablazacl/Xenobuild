
#pragma once 

#include <iostream>


namespace Xenobuild {
    enum class Toolchain {
        Clang,
        MicrosoftVC,
        AppleClang,
        GnuGCC
    };
    

    inline std::ostream& operator<< (std::ostream &ostream, const Toolchain toolchain) {
        switch (toolchain) {
        case Toolchain::Clang:
            ostream << "Toolchain::Clang";
            break;
            
        case Toolchain::MicrosoftVC:
            ostream << "Toolchain::MicrosoftVC";
            break;
            
        case Toolchain::AppleClang:
            ostream << "Toolchain::AppleClang";
            break;
            
        case Toolchain::GnuGCC:
            ostream << "Toolchain::GnuGCC";
            break;
            
        default:
            ostream << "<Toolchain: Missing case>";
            break;
        }
        
        return ostream;
    }    
}
