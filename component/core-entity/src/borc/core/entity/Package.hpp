
#pragma once 

#include <vector>
#include <string>

#include "Version.hpp"

namespace borc::core::entity {
	struct Package {
		std::string name;
		std::string description;
        Version version;
        std::string license;
        std::string author;
        std::string email;
        std::vector<std::string> definitionFiles;

        ~Package();
	};
}