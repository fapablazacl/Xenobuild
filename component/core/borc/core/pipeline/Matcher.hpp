
#pragma once 

#include <string>

namespace borc {
    class Matcher {
    public:
        virtual ~Matcher();

        virtual bool match(const std::string &fileName) = 0;
    };
}
