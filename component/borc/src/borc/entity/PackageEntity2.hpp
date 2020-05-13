
#pragma once 

#include <string>
#include <vector>
#include <boost/hana.hpp>

namespace borc {
    struct PackageEntity2 {
        std::string name;
        std::string description;
        std::string version;
        std::string license;
        std::string author;
        std::string email;
        std::vector<std::string> definition_path;
    }:
}

BOOST_HANA_ADAPT_STRUCT(borc::PackageEntity2, name, description, version, license, author, email, definition_path);
