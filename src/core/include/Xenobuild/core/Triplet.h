
#pragma once 

#include <ostream>
#include <string>
#include <boost/filesystem/path.hpp>

#include "ToolchainType.h"

#define XENO_OS_WINDOWS
#define XENO_OS_LINUX
#define XENO_OS_MACOS
#define XENO_OS_HOST
#define XENO_ARCH_NATIVE

#define XENO_OS     XENO_OS_HOST
#define XENO_ARCH   XENO_ARCH_NATIVE


namespace Xenobuild {
    enum class Arch {
        Native,
        X32,
        X64
    };
    
    inline std::ostream& operator<< (std::ostream &os, const Arch arch) {
        switch (arch) {
        case Arch::Native:
            os << "Arch::Native";
            break;
            
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
        Host,
        Windows,
        Linux,
        MacOS
    };
    
    inline std::ostream& operator<< (std::ostream &ostream, const OS os) {
        switch (os) {
        case OS::Host:
            ostream << "OS::Host";
            break;

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
    };
    
    inline std::ostream& operator<< (std::ostream &os, const Triplet &triplet) {
        os << "Triplet { ";
        os << triplet.platform << ", ";
        os << triplet.toolchainType << " }";
        
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

        case OS::Host:
            return "host";
        }

        return "os_unknown";
    }

    
    inline std::string computePathSuffix(const Arch arch) {
        switch (arch) {
        case Arch::X32:
            return "x86";

        case Arch::X64:
            return "x64";

        case Arch::Native:
            return "native";
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
