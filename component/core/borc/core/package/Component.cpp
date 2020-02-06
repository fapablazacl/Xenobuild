
#include "Component.hpp"

namespace borc {
    Component::Component(Package *parentPackage) 
        : package(parentPackage) {}

    Component::~Component() {}

    Component* Component::setName(const std::string &value) {
        name = value;

        return this;
    }

    Component* Component::setDescription(const std::string &value) {
        description = value;

        return this;
    }

    Component* Component::setType(const Component::Type &value) {
        type = value;

        return this;
    }

    Component* Component::setFiles(const std::vector<std::string> &values) {
        files = values;

        return this;
    }
}
