
#pragma once 

#include <iostream>

namespace Xenobuild {
    enum class ToolchainType {
        Clang,
        MicrosoftVC,
        AppleClang,
        GnuGCC
    };


    inline std::ostream& operator<< (std::ostream &ostream, const ToolchainType toolchain) {
        switch (toolchain) {
        case ToolchainType::Clang:
            ostream << "ToolchainType::Clang";
            break;
            
        case ToolchainType::MicrosoftVC:
            ostream << "ToolchainType::MicrosoftVC";
            break;
            
        case ToolchainType::AppleClang:
            ostream << "ToolchainType::AppleClang";
            break;
            
        case ToolchainType::GnuGCC:
            ostream << "ToolchainType::GnuGCC";
            break;
            
        default:
            ostream << "<ToolchainType: Missing case>";
            break;
        }
        
        return ostream;
    }
}
