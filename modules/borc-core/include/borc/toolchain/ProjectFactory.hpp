
#ifndef __BORC_CORE_PROJECTFACTORY_HPP__
#define __BORC_CORE_PROJECTFACTORY_HPP__

#include <string>
#include <memory>

namespace borc {
    class Project;
    class ProjectFactory {
	public:
		std::unique_ptr<Project> createProject(const std::string &projectPath) const;
	};
}

#endif
