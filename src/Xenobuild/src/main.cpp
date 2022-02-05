
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
#include <Xenobuild/ConfigureController.h>


using ControllerFactoryMap = std::map<std::string, std::unique_ptr<Xenobuild::ControllerFactory>>;


template <typename TController>
void appendFactoryController(ControllerFactoryMap &controllerFactoryMap, Xenobuild::Package &package) {
    auto name = TController::Name;
    auto factory = std::make_unique<Xenobuild::ConcreteControllerFactory<TController>>(package);
    
    const auto it = controllerFactoryMap.find(name);
    assert(it == controllerFactoryMap.end());
    
    controllerFactoryMap[name] = std::move(factory);
}


static ControllerFactoryMap createControllerFactoryMap(Xenobuild::Package &package) {
    ControllerFactoryMap result;
    
    appendFactoryController<Xenobuild::BuildController>(result, package);
    appendFactoryController<Xenobuild::SetupController>(result, package);
    appendFactoryController<Xenobuild::ConfigureController>(result, package);

    return result;
}


int main(int argc, char **argv) {
    std::fstream fs;
    
    fs.open("Xenobuild.yaml", std::ios::in);
    if (! fs.is_open()) {
        std::cerr << "There is no accesible Xenobuild.yaml file in the current folder" << std::endl;
        return EXIT_FAILURE;
    }
    
    Xenobuild::FileSystemPackageFactory packageFactory;
    Xenobuild::Package package = packageFactory.createPackage(fs);
    
    const ControllerFactoryMap controllerFactoryMap = createControllerFactoryMap(package);
    
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
