
#ifndef __BOK_TOOLCHAIN_MODULECHECKER_HPP__
#define __BOK_TOOLCHAIN_MODULECHECKER_HPP__

namespace bok {
    class Module;
    class ModuleChecker {
    public:
        virtual ~ModuleChecker();

        virtual bool check(const Module *module) const = 0;
    };
}

#endif
