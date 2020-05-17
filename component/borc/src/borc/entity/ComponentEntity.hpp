
#pragma once 

#include <string>
#include <vector>
#include <boost/hana.hpp>

namespace bok {
    std::string name;
    std::string description;
    std::string type;
    std::vector<std::string> languages;
    std::vector<std::string> sources;
    std::vector<std::string> dependencies;
}

BOOST_HANA_ADAPT_STRUCT(bok::PackageEntity2, name, description, type, languages, sources, dependencies);
