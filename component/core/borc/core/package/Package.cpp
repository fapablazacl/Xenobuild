
#include "Package.hpp"

#include "Component.hpp"

#include <stdexcept>

namespace borc {
    Package::~Package() {}
    
    Package* Package::setName(const std::string &value) {
        name = value;

        return this;
    }


    Package* Package::setDescription(const std::string &value) {
        description = value;

        return this;
    }


    Package* Package::setVersion(const Version &value) {
        version = value;

        return this;
    }


    Package* Package::setAuthor(const std::string &value) {
        author = value;

        return this;
    }


    Package* Package::setEmail(const std::string &value) {
        email = value;

        return this;
    }


    void Package::addComponent(Component *component) {
        const std::string name = component->getName();

        if (auto it = components.find(name); it != components.end()) {
            throw std::runtime_error("There is already a component with name '" + name + "' in the package '" + this->name + "'.");
        }

        components[name] = component;
        componentPtrs[name] = std::unique_ptr<Component>(component);
    }


    std::map<std::string, Component*> Package::getComponents() {
        return components;
    }


    int Package::getComponentCount() const {
        return static_cast<int>(components.size());
    }

    const Component* Package::getComponent(const std::string &name) const {
        const auto it = components.find(name);

        if (it == components.end()) {
            throw std::runtime_error("There is no component with name '" + name + "' within the package '" + name + "'.");
        }

        return it->second;
    }
}
