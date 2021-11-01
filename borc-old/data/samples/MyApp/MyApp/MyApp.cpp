
#include "MyLib01.hpp"
#include "MyLib02.hpp"

int main(int argc, char **argv) {
    myapp::Adder adder;
    myapp::Multipler multiplier;

    float result1 = adder.add(1, 2);
    float result2 = multiplier.multiply(2, 3);

    return 0;
}
