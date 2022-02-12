
#pragma once 

#include <ostream>
#include <string>
#include <boost/filesystem/path.hpp>

#include "Toolchain.h"


namespace Xenobuild {
    enum class Arch {
        X32,
        X64
    };
    
    inline std::ostream& operator<< (std::ostream &os, const Arch arch) {
        switch (arch) {
        case Arch::X32:
            os << "Arch::X32";
            break;
            
        case Arch::X64:
            os << "Arch::X64";
            break;
            
        default:
            os << "<Arch: Missing case>";
            break;
        }
        
        return os;
    }

    enum class OS {
        Windows,
        Linux,
        MacOS
    };
    
    
    inline std::ostream& operator<< (std::ostream &ostream, const OS os) {
        switch (os) {
        case OS::Windows:
            ostream << "OS::Windows";
            break;
            
        case OS::Linux:
            ostream << "OS::Linux";
            break;
            
        case OS::MacOS:
            ostream << "OS::MacOS";
            break;
            
        default:
            ostream << "<OS: Missing case>";
            break;
        }
        
        return ostream;
    }
    
    constexpr OS getHostOS() {
#if defined(_WIN32)
        return OS::Windows;
#elif defined(__APPLE__)
        return OS::MacOS;
#else
#error Current OS isn't supported.
#endif
    }
    

    struct Triplet {
        OS os;
        Arch arch;
        ToolchainType toolchain;
        
        static Triplet nativeHost() {
            switch (getHostOS()) {
            case OS::Windows:
                return { OS::Windows, Arch::X64, ToolchainType::MicrosoftVC };
                
            case OS::MacOS:
                return { OS::MacOS, Arch::X64, ToolchainType::AppleClang };
                
            case OS::Linux:
                return { OS::Linux, Arch::X64, ToolchainType::GnuGCC };
            }
        }
    };
    
    inline std::ostream& operator<< (std::ostream &os, const Triplet &triplet) {
        os << "Triplet { ";
        os << triplet.os << ", ";
        os << triplet.arch << ", ";
        os << triplet.toolchain << " }";
        
        return os;
    }


    inline std::string computePathSuffix(const OS os) {
        switch (os) {
        case OS::Windows:
            return "windows";

        case OS::Linux:
            return "linux";

        case OS::MacOS:
            return "macos";
        }

        return "host";
    }

    
    inline std::string computePathSuffix(const Arch arch) {
        switch (arch) {
        case Arch::X32:
            return "x86";

        case Arch::X64:
            return "x64";
        }

        return "native";
    }
    

    inline std::string computePathSuffix(const ToolchainType toolchain) {
        switch (toolchain) {
        case ToolchainType::AppleClang:
            return "appleclang";

        case ToolchainType::Clang:
            return "clang";

        case ToolchainType::MicrosoftVC:
            return "vc";
            
        case ToolchainType::GnuGCC:
            return "gcc";
        }

        return "cc";
    }

    inline std::string computePathSuffix(const Triplet& triplet) {
        return
            computePathSuffix(triplet.os) + "-" +
            computePathSuffix(triplet.arch) + "-" +
            computePathSuffix(triplet.toolchain);
    }
}
