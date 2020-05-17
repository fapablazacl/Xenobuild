
#ifndef __BORC_MODEL_VERSION_HPP__
#define __BORC_MODEL_VERSION_HPP__

#include <string>
#include <ostream>

namespace bok {
    struct Version {
        int major = 0;
        int minor = 0;
        int revision = 0;

        Version() {}

        Version(const int major, const int minor, const int revision) {
            this->major = major;
            this->minor = minor;
            this->revision = revision;
        }

        explicit Version(const std::string &str) {
            *this = Version::parse(str);
        }

        explicit operator std::string() const;

        bool operator< (const Version &rhs) const;
        
        bool operator>= (const Version &rhs) const {
            return ! (*this < rhs);
        }

        bool operator== (const Version &rhs) const;

        bool operator!= (const Version &rhs) const {
            return ! (*this == rhs);
        }

        static Version parse(const std::string &str);

        typedef std::string DefaultType;
    };    
}

inline std::ostream& operator << (std::ostream &os, const bok::Version &version) {
    return os << static_cast<std::string>(version);
}

#endif
