
#ifndef __BOK_TOOLCHAIN_MODULECHECKER_HPP__
#define __BOK_TOOLCHAIN_MODULECHECKER_HPP__

namespace bok {
    class Component;
    class ModuleChecker {
    public:
        virtual ~ModuleChecker();

        virtual bool check(const Component *component) const = 0;
    };
}

#endif
