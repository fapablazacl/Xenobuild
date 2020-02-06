
#include "Component.hpp"

namespace borc {
    Component::Component(Package *parentPackage) 
        : package(parentPackage) {}

    Component::~Component() {}

    Component* Component::setName(const std::string &value) {

        return this;
    }

    Component* Component::setDescription(const std::string &value) {

        return this;
    }

    Component* Component::setType(const Component::Type &value) {

        return this;
    }

    Component* Component::setFiles(const std::vector<std::string> &values) {

        return this;
    }
}
