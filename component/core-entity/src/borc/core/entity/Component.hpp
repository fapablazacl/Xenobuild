
#pragma once 

#include <string>

#include "Dependency.hpp"

namespace borc::core::entity {
	struct ComponentLanguage {
		std::string name;
		std::string dialect;
	};
	
	struct ComponentType {
		std::string name;
		std::string kind;
	};
	
	struct Component {
		std::string name;
		std::string description;
		ComponentType type;
		std::vector<ComponentLanguage> languages;
		std::vector<std::string> sources;
		std::vector<Dependency> dependencies;
	};
}
