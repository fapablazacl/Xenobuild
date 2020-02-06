
#include "Matcher.hpp"

namespace borc {
    Matcher::Matcher(const Pipeline *pipeline_, const std::string &fileTypeId_) 
        : pipeline(pipeline_), fileTypeId(fileTypeId_) {}


    Matcher::~Matcher() {}
}
