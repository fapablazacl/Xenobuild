
#ifndef __BORC_CORE_SERVICEFACTORY_HPP__
#define __BORC_CORE_SERVICEFACTORY_HPP__

#include <vector>
#include <memory>

namespace borc {
	class Compiler;
	class Linker;
	class RunService;
	class BuildService;
	class ServiceFactory {
	public:
		virtual ~ServiceFactory();

		virtual const Compiler* getCompiler() const = 0;

		virtual const Linker* getLinker() const = 0;
	};
} 

#endif
