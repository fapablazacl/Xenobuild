
#include <xb/core/Version.h>

#include <vector>
#include <stdexcept>
#include <boost/algorithm/string.hpp>

namespace Xenobuild {
    Version Version::parse(const std::string &str) {
        std::vector<std::string> results;
        boost::split(results, str, boost::is_any_of("."));

        if (results.size() > 3 || results.size() < 1) {
            throw std::runtime_error("Invalid version format");
        }

        return Version {
            std::atoi(results[0].c_str()),
            std::atoi(results[1].c_str()),
            std::atoi(results[2].c_str())
        };
    }

    bool Version::operator< (const Version &rhs) const {
        if (major >= rhs.major) {
            return false;
        }
        
        if (minor >= rhs.minor) {
            return false;
        }
        
        if (revision >= rhs.revision) {
            return false;
        }
        
        return true;
    }

    bool Version::operator== (const Version &rhs) const {
        return major == rhs.major && minor == rhs.minor && revision == rhs.revision;
    }

    Version::operator std::string() const {
        return std::to_string(major) + "." + 
            std::to_string(minor) + "." + 
            std::to_string(revision);
    }
}
