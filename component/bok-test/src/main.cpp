
#include <iostream>
#include <bok/cli/init/InitController.hpp>
#include <bok/cli/build/BuildController.hpp>
#include <bok/cli/configure/ConfigureController.hpp>
#include <bok/core/pipeline/DiskFileScanner.hpp>

int main() {
    // bok::DiskFileScanner{}.scan()

    std::cout << CMAKE_CURRENT_SOURCE_DIR << std::endl;

    return 0;
}
