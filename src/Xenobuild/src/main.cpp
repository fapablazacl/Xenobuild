
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <memory>

#include <Xenobuild/core/FileSystemPackageFactory.h>
#include <Xenobuild/ControllerFactory.h>
#include <Xenobuild/BuildController.h>
#include <Xenobuild/SetupController.h>


using ControllerFactoryMap = std::map<std::string, std::unique_ptr<Xenobuild::ControllerFactory>>;


static ControllerFactoryMap createControllerFactoryMap(Xenobuild::PackageFactory &packageFactory) {
    ControllerFactoryMap result;

    result[Xenobuild::BuildController::Name]
        = std::make_unique<Xenobuild::ConcreteControllerFactory<Xenobuild::BuildController>>(packageFactory);
    
    result[Xenobuild::SetupController::Name]
        = std::make_unique<Xenobuild::ConcreteControllerFactory<Xenobuild::SetupController>>(packageFactory);

    return result;
}


int main(int argc, char **argv) {
    Xenobuild::FileSystemPackageFactory packageFactory;

    const ControllerFactoryMap controllerFactoryMap = createControllerFactoryMap(packageFactory);
    
    if (argc < 2) {
        std::cerr << "No subcommand supplied." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<char*> args = {argv + 1, argv + argc};
    const std::string command = args[0];
    const auto factoryIt = controllerFactoryMap.find(command);

    if (factoryIt == controllerFactoryMap.end()) {
        std::cerr << "Unnown command " << command << std::endl;
        return EXIT_FAILURE;
    }

    assert(factoryIt->second);

    auto controller = factoryIt->second->createController(args.size(), args.data());

    controller->perform();

    return EXIT_SUCCESS;
}
