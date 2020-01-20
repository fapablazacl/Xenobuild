
#include <catch2/catch.hpp>
#include <borc/core/entity/Dependency.hpp>


TEST_CASE("Parse a Dependency correctly from 'jbeder/yaml-cpp'", "[borc.core.entity.Dependency]" ) {
    const auto dependency = borc::core::entity::Dependency("jbeder/yaml-cpp");
    REQUIRE(dependency.packageName == "jbeder");
    REQUIRE(dependency.dependencyVersion.has_value() == false);
    REQUIRE(dependency.componentNames.size() == 1);
    REQUIRE(dependency.componentNames[0] == "yaml-cpp");
}


TEST_CASE("Parsing correctly from 'boost>=1.70.0/[filesystem, system, process, program_options]'", "[borc.core.entity.Dependency]" ) {
    const auto dependency = borc::core::entity::Dependency("boost>=1.70.0/[filesystem, system, process, program_options]");

    REQUIRE(dependency.packageName == "boost");
    REQUIRE(dependency.componentNames.size() == 4);
    REQUIRE(dependency.componentNames[0] == "filesystem");
    REQUIRE(dependency.componentNames[1] == "system");
    REQUIRE(dependency.componentNames[2] == "process");
    REQUIRE(dependency.componentNames[3] == "program_options");

    REQUIRE(dependency.dependencyVersion.has_value() == true);
    REQUIRE(dependency.dependencyVersion->version == borc::core::entity::Version{1, 70, 0});
    REQUIRE(dependency.dependencyVersion->restriction == borc::core::entity::DependencyVersion::Restriction::GreaterOrEqual);
}


TEST_CASE("Parsing correctly from 'boost>=1.70.0/[filesystem]'", "[borc.core.entity.Dependency]" ) {
    const auto dependency = borc::core::entity::Dependency("boost>=1.70.0/[filesystem]");

    REQUIRE(dependency.packageName == "boost");
    REQUIRE(dependency.componentNames.size() == 1);
    REQUIRE(dependency.componentNames[0] == "filesystem");

    REQUIRE(dependency.dependencyVersion.has_value() == true);
    REQUIRE(dependency.dependencyVersion->version == borc::core::entity::Version{1, 70, 0});
    REQUIRE(dependency.dependencyVersion->restriction == borc::core::entity::DependencyVersion::Restriction::GreaterOrEqual);
}


TEST_CASE("Parsing correctly from 'boost>=1.70.0/filesystem'", "[borc.core.entity.Dependency]" ) {
    const auto dependency = borc::core::entity::Dependency("boost>=1.70.0/filesystem");

    REQUIRE(dependency.packageName == "boost");
    REQUIRE(dependency.componentNames.size() == 1);
    REQUIRE(dependency.componentNames[0] == "filesystem");

    REQUIRE(dependency.dependencyVersion.has_value() == true);
    REQUIRE(dependency.dependencyVersion->version == borc::core::entity::Version{1, 70, 0});
    REQUIRE(dependency.dependencyVersion->restriction == borc::core::entity::DependencyVersion::Restriction::GreaterOrEqual);
}


TEST_CASE("Parsing correctly from 'boost=1.60.0/[ranges]'", "[borc.core.entity.Dependency]" ) {
    const auto dependency = borc::core::entity::Dependency("boost=1.60.0/[ranges]");

    REQUIRE(dependency.packageName == "boost");
    REQUIRE(dependency.componentNames.size() == 1);
    REQUIRE(dependency.componentNames[0] == "ranges");

    REQUIRE(dependency.dependencyVersion.has_value() == true);
    REQUIRE(dependency.dependencyVersion->version == borc::core::entity::Version{1, 60, 0});
    REQUIRE(dependency.dependencyVersion->restriction == borc::core::entity::DependencyVersion::Restriction::Equal);
}


TEST_CASE("Parsing correctly from 'boost<=1.60.0/ranges'", "[borc.core.entity.Dependency]" ) {
    const auto dependency = borc::core::entity::Dependency("boost<=1.6/ranges");

    REQUIRE(dependency.packageName == "boost");
    REQUIRE(dependency.componentNames.size() == 1);
    REQUIRE(dependency.componentNames[0] == "ranges");

    REQUIRE(dependency.dependencyVersion.has_value() == true);
    REQUIRE(dependency.dependencyVersion->version == borc::core::entity::Version{1, 6});
    REQUIRE(dependency.dependencyVersion->restriction == borc::core::entity::DependencyVersion::Restriction::Equal);
}
