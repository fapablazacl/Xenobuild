
#include <catch2/catch.hpp>
#include <borc/core/package/Version.hpp>

TEST_CASE("borc::Version class: Constructor correctly initialize the values", "[borc.core.package.Version]" ) {
    const auto v1 = borc::Version{1, 2, 3};
    REQUIRE(v1.major == 1);
    REQUIRE(v1.minor == 2);
    REQUIRE(v1.revision == 3);

    const auto v2 = borc::Version{3, 4, 1};
    REQUIRE(v2.major == 3);
    REQUIRE(v2.minor == 4);
    REQUIRE(v2.revision == 1);
}


TEST_CASE("Comparison operator correctly check the values", "[borc.core.entity.Version]" ) {
    using borc::Version;

    REQUIRE(Version{1, 2, 3} == Version{1, 2, 3});
    REQUIRE(Version{4, 3, 1} == Version{4, 3, 1});

    REQUIRE_FALSE(Version{1, 2, 3} == Version{5, 2, 3});
    REQUIRE_FALSE(Version{1, 2, 3} == Version{1, 6, 3});
    REQUIRE_FALSE(Version{1, 2, 3} == Version{1, 2, 7});
}
