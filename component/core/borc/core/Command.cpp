
#include "Command.hpp"

namespace borc {
    Command::Command(const std::string &name) {
        this->setName(name);
    }


    Command::Command(const std::string &name, const std::vector<std::string> &parameters) {
        this->setName(name);
        this->setParameters(parameters);
    }


    Command::~Command() {}


    void Command::setName(const std::string value) {
        name = value;
    }

    
    void Command::setParameters(const std::vector<std::string> &value) {
        parameters = value;
    }            
}
