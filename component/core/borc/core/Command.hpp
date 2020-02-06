
#pragma once 

#include <string>
#include <vector>

namespace borc {
    class Command {
    public:
        explicit Command(const std::string &name);

        explicit Command(const std::string &name, const std::vector<std::string> &parameters);

        ~Command();

        void setName(const std::string value);

        void setParameters(const std::vector<std::string> &value);
            
        std::string getName() const {
            return name;
        }

        std::vector<std::string> getParameters() const {
            return parameters;
        }

    private:
        std::string name;
        std::vector<std::string> parameters;
    };
}
