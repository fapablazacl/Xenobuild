
#include <borc/model/Version.hpp>

#include <vector>
#include <stdexcept>
#include <boost/algorithm/string.hpp>

namespace borc {
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
}
