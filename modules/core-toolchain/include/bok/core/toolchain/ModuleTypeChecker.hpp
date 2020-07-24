
#ifndef __BOK_TOOLCHAIN_MODULETYPECHECKER_HPP__
#define __BOK_TOOLCHAIN_MODULETYPECHECKER_HPP__

#include <bok/core/toolchain/ModuleChecker.hpp>
#include <bok/core/package/Module.hpp>
#include <set>
#include <initializer_list>

namespace bok {
    class ModuleTypeChecker : public ModuleChecker {
    public:
        ModuleTypeChecker(const std::initializer_list<Component::Type> &types);

        virtual ~ModuleTypeChecker();

        virtual bool check(const Component *component) const override;

    private:
        std::set<Component::Type> types;
    };
}

#endif
