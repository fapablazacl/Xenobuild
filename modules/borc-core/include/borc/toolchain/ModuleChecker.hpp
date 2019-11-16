
#ifndef __BORC_TOOLCHAIN_MODULECHECKER_HPP__
#define __BORC_TOOLCHAIN_MODULECHECKER_HPP__

namespace borc {
    class Module;
    class ModuleChecker {
	public:
		virtual ~ModuleChecker();

		virtual bool check(const Module *module) const = 0;
	};
}

#endif