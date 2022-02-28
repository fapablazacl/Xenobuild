
#pragma once 

#include <iostream>
#include <string>
#include <boost/optional.hpp>


namespace Xenobuild {
    enum class ToolchainType {
        Clang,
        MicrosoftVC,
        AppleClang,
        GnuGCC
    };

    constexpr ToolchainType getCurrentToolchainType() {
#if defined(_MSC_VER)
        return ToolchainType::MicrosoftVC;
#else
#if defined(__clang__)
#if defined(__apple_build_version__)
        return ToolchainType::AppleClang;
#else
        return ToolchainType::Clang;
#endif
#elif defined(__GNUC__)
        return ToolchainType::GnuGCC;
#endif
#endif
    }
    
    boost::optional<std::string> encode(const ToolchainType toolchainType);

    boost::optional<ToolchainType> decode(const std::string& value);

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
