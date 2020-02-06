
#pragma once 

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "Version.hpp"

namespace borc {
    class Component;

    class Package {
    public:
        ~Package();

        Package* setName(const std::string &value);
        Package* setDescription(const std::string &value);
        Package* setVersion(const Version &value);
        Package* setAuthor(const std::string &value);
        Package* setEmail(const std::string &value);

        std::string getName() const {
            return name;
        }

        std::string getDescription() const {
            return description;
        }

        Version getVersion() const {
            return version;
        }

        std::string getAuthor() const {
            return author;
        }

        std::string getEmail() const {
            return email;
        }

        template<typename ComponentImpl>
        ComponentImpl* addComponent(const std::string &name) {
            auto component = new ComponentImpl(this);
            component->setName(name);

            this->addComponent(name, component);

            return component;
        }
        
        std::map<std::string, Component*> getComponents();

        int getComponentCount() const;

        const Component* getComponent(const std::string &name) const;

    protected:
        void addComponent(Component *component);

    private:
        std::string name;
        std::string description;
        Version version;
        std::string author;
        std::string email;
        std::map<std::string, std::unique_ptr<Component>> componentPtrs;
        std::map<std::string, Component*> components;
    };
}
