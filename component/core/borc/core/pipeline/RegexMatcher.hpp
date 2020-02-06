
#pragma once 

#include "Matcher.hpp"

namespace borc {
    class RegexMatcher : public Matcher {
    public:
        explicit RegexMatcher(const Pipeline *pipeline, const std::string &fileTypeId);

        virtual ~RegexMatcher();

        virtual bool match(const std::string &fileName) override;

        void setPattern(const std::string &value);

        std::string getPattern() const {
            return regexPattern;
        }

    private:
        std::string regexPattern;
    };
}
