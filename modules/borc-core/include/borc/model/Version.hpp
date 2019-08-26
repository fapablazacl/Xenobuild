
#ifndef __BORC_MODEL_VERSION_HPP__
#define __BORC_MODEL_VERSION_HPP__

#include <string>

namespace borc {
    struct Version {
        int major = 0;
        int minor = 0;
        int revision = 0;

        static Version parse(const std::string &str);
    };
}

#endif
