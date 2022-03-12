
#include <Xenobuild/ConfigureController.h>

#include <boost/optional/optional_io.hpp>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageManager.h>
#include <Xenobuild/core/DependencyManager.h>
#include <Xenobuild/core/Context.h>
#include <Xenobuild/core/Toolchain.h>
#include <Xenobuild/core/Triplet.h>
#include <Xenobuild/core/CMakeBuildSystem.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace Xenobuild {
    class MockCommandExecutor : public CommandExecutor {
    public:
        MOCK_METHOD(CommandResult, execute, (const CommandX&), (override));
        MOCK_METHOD(CommandResult, execute, (const CommandBatch&), (override));
    };

    class MockPackageManager : public PackageManager {
    public:
        explicit MockPackageManager(CommandExecutor &executor) : PackageManager(executor, "", "") {}

        MOCK_METHOD(bool, configure, (
            const Package &package, 
            const Toolchain &toolchain, 
            const Triplet &triplet, 
            const CMakeBuildType buildType, 
            const DependencyManager &dependencyManager), (override));
    };

    class MockDependencyManager : public DependencyManager {
    public:
        explicit MockDependencyManager(CommandExecutor &executor) : DependencyManager(executor, "", "") {}

        MOCK_METHOD(bool, download, (const Dependency& dependency), (const override));

        MOCK_METHOD(bool, configure, (
            const Dependency& dependency, 
            const Toolchain &toolchain, 
            const CMakeBuildType buildType, 
            const boost::optional<CMakeGenerator> generator), (override));

        MOCK_METHOD(bool, build, (
            const Dependency& dependency, 
            const Toolchain &toolchain, 
            const CMakeBuildType buildType), (override));

        MOCK_METHOD(bool, install, (
            const Dependency& dependency, 
            const Toolchain &toolchain, 
            const CMakeBuildType buildType), (override));
    };

    namespace {
        // The fixture for testing class Foo.
        class ConfigureControllerTest : public ::testing::Test {
        protected:
            ConfigureControllerTest() {}
            ~ConfigureControllerTest() override {}
        };

        using ::testing::Return;
        using ::testing::AtLeast;
        using ::testing::_;

        // Tests that the ConfigureControllerTest::performImpl() method does MethodPerformImplDoesConfigurationForDebugAndRelease.
        TEST_F(ConfigureControllerTest, MethodPerformImplDoesConfigurationForDebugAndRelease) {
            Package package;
            package.name = "Test01";

            Toolchain toolchain{ {}, "" };
            Context context { package, toolchain };

            MockCommandExecutor executor;

            ON_CALL(executor, execute(CommandX{})).WillByDefault(Return(CommandResult{0, {}, {}}));
            ON_CALL(executor, execute(CommandBatch{})).WillByDefault(Return(CommandResult{0, {}, {}}));

            MockPackageManager packageManager{executor};
            MockDependencyManager dependencyManager{executor};

            ON_CALL(packageManager, configure(_, _, _, _, _)).WillByDefault(Return(true));
            EXPECT_CALL(packageManager, configure(_, _, _, _, _)).Times(AtLeast(2));

            ConfigureControllerInput input;
            ConfigureController controller{context, input};
            controller.performImpl(packageManager, dependencyManager);
        }
    }
}
