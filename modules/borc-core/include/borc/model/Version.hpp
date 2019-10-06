
#ifndef __BORC_MODEL_VERSION_HPP__
#define __BORC_MODEL_VERSION_HPP__

#include <string>
#include <ostream>

namespace borc {
    struct Version {
        int major = 0;
        int minor = 0;
        int revision = 0;

        static Version parse(const std::string &str);
    };    
}

inline std::ostream& operator << (std::ostream &os, const borc::Version &version) {
    return os << version.major << "." << version.minor << "." << version.revision;
}

#endif
