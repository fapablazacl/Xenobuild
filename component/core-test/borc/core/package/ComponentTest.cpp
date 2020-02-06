
#include <catch2/catch.hpp>
#include <borc/core/package/Component.hpp>

TEST_CASE("borc::Component class: Setters and getters in general behave correctly", "[borc.core.package.Package]" ) {
    auto component = std::make_unique<borc::Component>(nullptr);

    component->setName("core")
        ->setDescription("Central logic for building systems!")
        ->setType({"library", "static"})
        ->setFiles({
            "borc/core/Component.cpp",
            "borc/core/Package.cpp",
        });

    REQUIRE(component->getPackage() == nullptr);
    REQUIRE(component->getName() == "core");
    REQUIRE(component->getDescription() == "Central logic for building systems!");
    REQUIRE(component->getType() == borc::Component::Type{"library", "static"});
    REQUIRE(component->getFiles() == std::vector<std::string>{
           "borc/core/Component.cpp",
           "borc/core/Package.cpp"
    });
}
