
#pragma once 

#include <vector>
#include <string>

namespace borc {
    struct Version {
        int major = 0;
        int minor = 0;
        int revision = 0;

        Version();

        Version(const int major, const int minor, const int revision);

        bool operator== (const Version &rhs) const;
    };
}
