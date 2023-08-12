
#include "Borc.hpp"

#include <iostream>

namespace borc {

    struct Borc::Private {
        int lala;
    };

    Borc::Borc() 
        : m_impl(new Borc::Private()) {}

    Borc::~Borc() {}

    int Borc::run(int argc, char **argv) {
        std::cout << "Hello, World!" << std::endl;
        std::cin.get();

        return 0;
    }
}

int main(int argc, char **argv) {
    borc::Borc borc_app;
    return borc_app.run(argc, argv);
}
