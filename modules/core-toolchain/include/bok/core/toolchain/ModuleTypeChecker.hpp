
#ifndef __BOK_TOOLCHAIN_MODULETYPECHECKER_HPP__
#define __BOK_TOOLCHAIN_MODULETYPECHECKER_HPP__

#include <bok/core/toolchain/ModuleChecker.hpp>
#include <bok/core/package/Module.hpp>
#include <set>
#include <initializer_list>

namespace bok {
    class ModuleTypeChecker : public ModuleChecker {
    public:
        ModuleTypeChecker(const std::initializer_list<Module::Type> &types);

        virtual ~ModuleTypeChecker();

        virtual bool check(const Module *component) const override;

    private:
        std::set<Module::Type> types;
    };
}

#endif
