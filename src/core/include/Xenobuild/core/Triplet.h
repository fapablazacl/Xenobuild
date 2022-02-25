
#pragma once 

#include <ostream>
#include <string>
#include <boost/filesystem/path.hpp>

#include "ToolchainType.h"


namespace Xenobuild {
    enum class Arch {
        Native,
        X32,
        X64,
        IA64,
        ARM64
    };
    
    
    constexpr Arch getNativeArch() {
#if defined(_M_IX86) || defined(i386) || defined(__i386__) || defined(__i386)
        return Arch::X32;
#elif defined(_M_X64) || defined(__x86_64__)
        return Arch::X64;
#elif defined(_M_IA64)
        return Arch::IA64;
#elif defined(_M_ARM64) || defined(__aarch64__)
        return Arch::ARM64;
#else
        return Arch::Native;
#endif
    }
    
    
    inline std::ostream& operator<< (std::ostream &os, const Arch arch) {
        switch (arch) {
        case Arch::Native: os << "Arch::Native"; break;
        case Arch::X32: os << "Arch::X32"; break;
        case Arch::X64: os << "Arch::X64"; break;
        case Arch::IA64: os << "Arch::IA64"; break;
        case Arch::ARM64: os << "Arch::ARM64"; break;
        default: os << "<Arch: Missing case>"; break;
        }
        
        return os;
    }

    enum class OS {
        Host,
        Windows,
        Linux,
        MacOS,
        Android,
        Fuchsia
    };
    
    constexpr OS getHostOS() {
#if defined(__linux__)
#  if defined(__ANDROID__)
        return OS::Android:
#  else
        return OS::Linux;
#  endif
#elif defined(__APPLE__)
        return OS::MacOS;
#elif defined(_WIN32)
        return OS::Windows;
#elif defined(__Fuchsia__)
        return OS::Fuchsia;
#else
        return OS::Host;
#endif
    }
    
    inline std::ostream& operator<< (std::ostream &ostream, const OS os) {
        switch (os) {
        case OS::Host: ostream << "OS::Host"; break;
        case OS::Windows: ostream << "OS::Windows"; break;
        case OS::Linux: ostream << "OS::Linux"; break;
        case OS::MacOS: ostream << "OS::MacOS"; break;
        case OS::Android: ostream << "OS::Android"; break;
        case OS::Fuchsia: ostream << "OS::Fuchsia"; break;
        default: ostream << "<OS: Missing case>"; break;
        }
        
        return ostream;
    }
    
    struct Platform {
        OS os = OS::Host;
        Arch arch = Arch::Native;
    };

    inline std::ostream& operator<< (std::ostream &os, const Platform &platform) {
        os << "Platform { ";
        os << platform.os << ", ";
        os << platform.arch << " }";
        
        return os;
    }

    struct Triplet {
        Platform platform;
        ToolchainType toolchainType;
        
        static Triplet createDefaultTriplet() {
            Triplet triplet;
            
            triplet.platform.os = getHostOS();
            triplet.platform.arch = getNativeArch();
            
            switch (triplet.platform.os) {
            case OS::Windows:
                triplet.toolchainType = ToolchainType::MicrosoftVC;
                break;
                
            case OS::MacOS:
                triplet.toolchainType = ToolchainType::AppleClang;
                break;
                
            default:
                triplet.toolchainType = ToolchainType::GnuGCC;
                break;
            }
            
            return triplet;
        }
    };
    
    inline std::ostream& operator<< (std::ostream &os, const Triplet &triplet) {
        os << "Triplet { ";
        os << triplet.platform << ", ";
        os << triplet.toolchainType << " }";
        
        return os;
    }

    inline std::string computePathSuffix(const OS os) {
        switch (os) {
        case OS::Windows: return "windows";
        case OS::Linux: return "linux";
        case OS::MacOS: return "macos";
        case OS::Android: return "android";
        case OS::Fuchsia: return "fuchsia";
        case OS::Host: return "host";
        }

        return "os_unknown";
    }

    
    inline std::string computePathSuffix(const Arch arch) {
        switch (arch) {
        case Arch::X32: return "x86";
        case Arch::X64: return "x64";
        case Arch::IA64: return "IA64";
        case Arch::ARM64: return "ARM64";
        case Arch::Native: return "native";
        }

        return "arch_unknown";
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

        return "toolchain_unknown";
    }

    inline std::string computePathSuffix(const Triplet& triplet) {
        return
            computePathSuffix(triplet.platform.os) + "-" +
            computePathSuffix(triplet.platform.arch) + "-" +
            computePathSuffix(triplet.toolchainType);
    }
}
