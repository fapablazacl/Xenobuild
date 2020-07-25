
#include <bok/core/toolchain/ModuleTypeChecker.hpp>

namespace bok {
    ModuleTypeChecker::ModuleTypeChecker(const std::initializer_list<Module::Type> &types) {
        for (Module::Type type : types) {
            this->types.insert(type);
        }
    }

    ModuleTypeChecker::~ModuleTypeChecker() {}

    bool ModuleTypeChecker::check(const Module *component) const {
        for (Module::Type type : types) {
            if (type == component->getType()) {
                return true;
            }
        }

        return false;
    }
}
