
#include "RegexMatcher.hpp"

namespace borc {
    RegexMatcher::RegexMatcher(const Pipeline *pipeline, const std::string &fileTypeId)
        : Matcher(pipeline, fileTypeId){}


    RegexMatcher::~RegexMatcher() {}


    bool RegexMatcher::match(const std::string &fileName) {
        // TODO: Add implementation
        return true;
    }


    void RegexMatcher::setPattern(const std::string &value) {
        regexPattern = value;
    }
}
