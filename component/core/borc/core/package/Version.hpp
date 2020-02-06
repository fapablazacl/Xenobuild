
#pragma once 

#include <vector>
#include <string>

namespace borc {
    struct Version {
        int major = 0;
        int minor = 0;
        int revision = 0;

        Version();
    };
}
