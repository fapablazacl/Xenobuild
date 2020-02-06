
#pragma once 

#include "Matcher.hpp"

namespace borc {
    class RegexMatcher : public Matcher {
    public:
        explicit RegexMatcher(const std::string &name, const std::string &regexPattern);

        virtual ~RegexMatcher();

        virtual bool match(const std::string &fileName) override;

    private:
        std::string name;
        std::string regexPattern;
    };
}
