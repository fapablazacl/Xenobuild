
#ifndef __BORC_TOOLCHAIN_MODULETYPECHECKER_HPP__
#define __BORC_TOOLCHAIN_MODULETYPECHECKER_HPP__

#include <borc/toolchain/ModuleChecker.hpp>

#include <borc/model/Module.hpp>
#include <set>
#include <initializer_list>

namespace bok {
    class ModuleTypeChecker : public ModuleChecker {
    public:
        ModuleTypeChecker(const std::initializer_list<Module::Type> &types);

        virtual ~ModuleTypeChecker();

        virtual bool check(const Module *module) const override;

    private:
        std::set<Module::Type> types;
    };
}

#endif
