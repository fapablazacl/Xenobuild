
#include <borc/model/Context.hpp>

namespace borc {
    Context::Context() {}

    Context::~Context() {}

    void Context::registerPackage(const Package *package) {
        packages.push_back(package);
    }

    void Context::unregisterPackage(const Package *package) {
        // TODO: Add implementation
        throw std::runtime_error("Context::unregisterPackage: Not implemented");
    }

    const Artifact* Context::findModule(const std::string &identifier) const {
        auto it = moduleMap.find(identifier);

        if (it == moduleMap.end()) {
            return nullptr;
        }

        return it->second;
    }
}
