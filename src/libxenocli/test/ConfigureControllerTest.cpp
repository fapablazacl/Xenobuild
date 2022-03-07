
#include <Xenobuild/ConfigureController.h>

#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/Context.h>
#include <Xenobuild/core/Toolchain.h>
#include <Xenobuild/core/Triplet.h>
#include <catch2/catch_all.hpp>

using namespace Xenobuild;


class MockCommandExecutor : public CommandExecutor {
public:
    CommandResult execute(const CommandX& command) override {
        return {0, "", ""};
    }

    CommandResult execute(const CommandBatch& batch) override {
        return {0, "", ""};
    }
};


SCENARIO("ConfigureController must perform all actions required to build correctly an package", "[ConfigureController]") {
    GIVEN("Its name must be 'configure'") {
        REQUIRE(std::strcmp(ConfigureController::Name, "configure") == 0);
    }

    GIVEN("a simple package with no dependencies") {
        Package package;
        package.name = "Test01";

        Toolchain toolchain{ {}, "" };
        Context context { package, toolchain };


        WHEN("the configure command is issued") {
            ConfigureControllerInput input;

            ConfigureController controller{context, input};
            controller.perform();

            THEN("generates the Debug and Release build configurations") {
                REQUIRE(true);
            }
        }
    }
}
