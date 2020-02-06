
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

            bool operator== (const Type &other) const {
                return std::tuple(category, subCategory) == std::tuple(other.category, other.subCategory);
            }
        };

        struct Language {
            std::string name;
            std::string dialect;

            bool operator== (const Language &other) const {
                return std::tuple(name, dialect) == std::tuple(other.name, other.dialect);
            }
        };

        explicit Component(Package *parentPackage);

        ~Component();

        Component* setName(const std::string &value);

        Component* setDescription(const std::string &value);

        Component* setType(const Type &value);

        Component* setFiles(const std::vector<std::string> &values);

        Component* setLanguage(const Language &value);

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
        
        Language getLanguage() const {
            return language;
        }

    private:
        Package *package = nullptr;
        std::string name;
        std::string description;
        Type type;
        Language language;

        std::vector<std::string> files;
    };
}
