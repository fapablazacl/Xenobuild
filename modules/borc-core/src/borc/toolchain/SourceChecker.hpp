
#ifndef __BORC_TOOLCHAIN_SOURCECHECKER_HPP__
#define __BORC_TOOLCHAIN_SOURCECHECKER_HPP__

#include <string>
#include <set>
#include <initializer_list>
#include <boost/filesystem/path.hpp>

namespace borc {
    class SourceChecker {
    public:
        SourceChecker(const std::initializer_list<std::string> &wildcards);

        bool check(const boost::filesystem::path &filePath) const;

    private:
        std::set<std::string> wildcards;
    };
}

#endif
