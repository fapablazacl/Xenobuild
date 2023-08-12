
#include <UnitTest++/UnitTest++.h>

#include <borc/BorcFile.hpp>

int main(int argc, char **argv) {
    auto file = borc::BorcFile::get("../borcfile");

    return UnitTest::RunAllTests();
}
