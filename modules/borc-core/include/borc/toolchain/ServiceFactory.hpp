
#ifndef __BORC_CORE_SERVICEFACTORY_HPP__
#define __BORC_CORE_SERVICEFACTORY_HPP__

#include <vector>
#include <memory>

namespace borc {
	class Compiler;
	class Linker;
	class SourceChecker;
	class ArtifactChecker;

	class ServiceFactory {
	public:
		virtual ~ServiceFactory();

		virtual const std::vector<std::pair<SourceChecker*, const Compiler*>> getCompilers() const = 0;

		virtual const std::vector<std::pair<ArtifactChecker*, const Linker*>> getLinkers() const = 0;
	};
} 

#endif
