
#pragma once 

#include <string>
#include <vector>

namespace borc {
    class Package;
    class Component {
    public:
        struct Type {
            std::string category;
            std::string subCategory;
        };

        explicit Component(Package *parentPackage);

        ~Component();

        Component* setName(const std::string &value);

        Component* setDescription(const std::string &value);

        Component* setType(const Type &value);

        Component* setFiles(const std::vector<std::string> &values);

        const Package* getPackage() const {
            return package;
        }

        std::string getName() const {
            return name;
        }
        
        std::string getDescription() const {
            return description;
        }
                
        Type getType() const {
            return type;
        }

        std::vector<std::string> getFiles() const {
            return files;
        }

    private:
        Package *package = nullptr;
        std::string name;
        std::string description;
        Type type;

        std::vector<std::string> files;
    };
}
