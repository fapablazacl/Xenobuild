
#include <catch2/catch.hpp>
#include <borc/core/package/Package.hpp>


TEST_CASE("borc::Package class: Setters and getters in general behave correctly", "[borc.core.package.Package]" ) {
    auto package = borc::Package();

    REQUIRE(package.setName("borc")->getName() == "borc");
    REQUIRE(package.setVersion({1, 2, 5})->getVersion() == borc::Version{1, 2, 5});

    REQUIRE(package.setDescription("A simple and pragmatic package development manager for C++")->getDescription() == "A simple and pragmatic package development manager for C++");
    REQUIRE(package.setAuthor("Phillip Andrews")->getAuthor() == "Phillip Andrews");
    REQUIRE(package.setEmail("author_nickname@gmail.com")->getEmail() == "author_nickname@gmail.com");
}


TEST_CASE("Component management (add and retrieval) is handled correctly.", "[borc.core.package.Package]" ) {
    auto package = std::make_unique<borc::Package>();

    package->setName("borc")
        ->setVersion({1, 2, 5})
        ->setDescription("A simple and pragmatic package development manager for C++")
        ->setAuthor("Phillip Andrews")
        ->setEmail("author_nickname@gmail.com");

    auto coreComponent = package->addComponent<borc::Component>("core");
    auto utilComponent = package->addComponent<borc::Component>("util");
    auto components = package->getComponents();

    REQUIRE(coreComponent->getPackage() == package.get());
    REQUIRE(utilComponent->getPackage() == package.get());

    REQUIRE(package->getComponentCount() == 2);
    REQUIRE(components.size() == 2);
    REQUIRE(coreComponent == components["core"]);
    REQUIRE(utilComponent == components["util"]);
    REQUIRE(coreComponent->getName() == "core");
    REQUIRE(utilComponent->getName() == "util");
}
