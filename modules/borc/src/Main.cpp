
#include <iostream>
#include <stdexcept>
#include <vector>

#include "borc/cli/Controller.hpp"
#include "borc/cli/ControllerFactory.hpp"

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            throw std::runtime_error("No subcommand supplied.");
        }

        std::vector<char*> args = {argv + 1, argv + argc};

        borc::ControllerFactory controllerFactory;
        auto controller = controllerFactory.create(args[0]);

        controller->perform(args.size(), args.data());
    
        return 0;
    } catch (const std::exception &exp) {
        std::cout << "ERROR: " << exp.what() << std::endl;

        return 1;
    }

    return 0;
}

/*
#include <iostream>
#include <boost/hana.hpp>

struct Address {
    std::string street;
    int number;
};

struct Person {
    std::string name;
    int age;

    Address address;
};


BOOST_HANA_ADAPT_STRUCT(Address, street, number);
BOOST_HANA_ADAPT_STRUCT(Person, name, age, address);

template<typename T>
class JsonConverter {
public:
    void serialize() {
        
    }
};

int main(int argc, char **argv) {
    Person person {"John", 30, {"San Pancho", 242}};

    boost::hana::for_each(person, [](auto pair) {
        std::string fieldName = boost::hana::to<char const*>(boost::hana::first(pair));
        auto fieldValue = boost::hana::second(pair);

        std::cout << fieldName << ":" << fieldValue << std::endl;
    });

    return 0;
}
*/
