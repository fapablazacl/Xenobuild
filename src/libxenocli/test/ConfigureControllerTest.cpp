
#include <Xenobuild/ConfigureController.h>

#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageManager.h>
#include <Xenobuild/core/DependencyManager.h>
#include <Xenobuild/core/Context.h>
#include <Xenobuild/core/Toolchain.h>
#include <Xenobuild/core/Triplet.h>
#include <catch2/catch_all.hpp>

using namespace Xenobuild;


class MockCommandExecutor : public CommandExecutor {
public:
    CommandResult execute(const CommandX& command) override {
        return { 0, {}, {} };
    }

    CommandResult execute(const CommandBatch& batch) override {
        return { 0, {}, {} };
    }
};


class MockPackageManager : public PackageManager {
public:
    explicit MockPackageManager(CommandExecutor &executor,
                            const std::string& prefixPath,
                            const std::string &installSuffix) :                     
        PackageManager(executor, prefixPath, installSuffix) {}


    virtual ~MockPackageManager() {}
    
    bool configure(
        const Package &package, 
        const Toolchain &toolchain, 
        const Triplet &triplet, 
        const CMakeBuildType buildType, 
        const DependencyManager &dependencyManager) override {
        
        configureCalled = true;
        
        return true;
    }

public:
    mutable bool configureCalled = false;
};


class MockDependencyManager : public DependencyManager {
public:
    explicit MockDependencyManager(CommandExecutor &executor,
                                const std::string& prefixPath,
                                const std::string &installSuffix) : 
        DependencyManager(executor, prefixPath, installSuffix) {}

    virtual ~MockDependencyManager() {}

    bool download(
        const Dependency& dependency) const override {
        
        downloadCalled = true;

        return true;
    }

    bool configure(
        const Dependency& dependency, 
        const Toolchain &toolchain, 
        const CMakeBuildType buildType, 
        const boost::optional<CMakeGenerator> generator) override {

        configureCalled = true;

        return true;
    }

    bool build(
        const Dependency& dependency, 
        const Toolchain &toolchain, 
        const CMakeBuildType buildType) override {

        buildCalled = true;

        return true;
    }

    bool install(
        const Dependency& dependency, 
        const Toolchain &toolchain, 
        const CMakeBuildType buildType) override {

        installCalled = true;

        return true;
    }

public:
    mutable bool downloadCalled = false;
    mutable bool configureCalled = false;
    mutable bool buildCalled = false;
    mutable bool installCalled = false;
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
            MockCommandExecutor executor;
            MockPackageManager packageManager{executor, "", ""};
            MockDependencyManager dependencyManager{executor, "", ""};

            ConfigureControllerInput input;

            ConfigureController controller{context, input};
            controller.performImpl(packageManager, dependencyManager);

            THEN("generates the Debug and Release build configurations, with no dependencies") {
                REQUIRE(dependencyManager.downloadCalled == false);
                REQUIRE(dependencyManager.configureCalled == false);
                REQUIRE(dependencyManager.buildCalled == false);
                REQUIRE(dependencyManager.installCalled == false);
            }
        }
    }
}
