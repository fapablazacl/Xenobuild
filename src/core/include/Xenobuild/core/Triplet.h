
#pragma once 



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
    
    
    struct Triplet {
        OS os;
        Arch arch;
        Toolchain toolchain;
        
        static Triplet nativeHost() {
            switch (getHostOS()) {
            case OS::Windows:
                return { OS::Windows, Arch::X64, Toolchain::MicrosoftVC };
                
            case OS::MacOS:
                return { OS::MacOS, Arch::X64, Toolchain::AppleClang };
                
            case OS::Linux:
                return { OS::Linux, Arch::X64, Toolchain::GnuGCC };
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
    

    inline std::string computePathSuffix(const Toolchain toolchain) {
        switch (toolchain) {
        case Toolchain::AppleClang:
            return "appleclang";

        case Toolchain::Clang:
            return "clang";

        case Toolchain::MicrosoftVC:
            return "vc";
            
        case Toolchain::GnuGCC:
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
    
    /**
     * @brief Get the home folder of the current user.
     */
     inline boost::filesystem::path getUserPath() {
        switch (getHostOS()) {
            case OS::Windows:
                return std::getenv("USERPROFILE");
                
            case OS::MacOS:
            case OS::Linux:
                return std::getenv("HOME");
                
            default:
                return ".";
        }
    }    
}
