
#ifndef __BORC_MODEL_RUNSERVICE_HPP__
#define __BORC_MODEL_RUNSERVICE_HPP__

#include "Predef.h"

namespace borc {
	class Compiler;
	class Linker;
	class Module;

	class RunService {
	public:
		explicit RunService(const Compiler *compiler, const Linker *linker);

		void runModule(const Module *module);

	private:
		const Compiler *compiler;
		const Linker *linker;
	};
}

#endif
