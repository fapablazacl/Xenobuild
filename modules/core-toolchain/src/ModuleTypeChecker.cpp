
#include <bok/core/toolchain/ModuleTypeChecker.hpp>

namespace bok {
    ModuleTypeChecker::ModuleTypeChecker(const std::initializer_list<Component::Type> &types) {
        for (Component::Type type : types) {
            this->types.insert(type);
        }
    }

    ModuleTypeChecker::~ModuleTypeChecker() {}

    bool ModuleTypeChecker::check(const Component *component) const {
        for (Component::Type type : types) {
            if (type == component->getType()) {
                return true;
            }
        }

        return false;
    }
}
