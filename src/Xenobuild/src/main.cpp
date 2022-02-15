
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <memory>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

#include <Xenobuild/core/FileSystemPackageFactory.h>
#include <Xenobuild/core/Context.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/Toolchain.h>
#include <Xenobuild/core/Dependency.h>
#include <Xenobuild/ControllerFactory.h>
#include <Xenobuild/BuildController.h>
#include <Xenobuild/SetupController.h>
#include <Xenobuild/ConfigureController.h>


using ControllerFactoryMap = std::map<std::string, std::unique_ptr<Xenobuild::ControllerFactory>>;


template <typename TController>
void appendFactoryController(ControllerFactoryMap &controllerFactoryMap, Xenobuild::Context &context) {
    auto name = TController::Name;
    auto factory = std::make_unique<Xenobuild::ConcreteControllerFactory<TController>>(context);
    
    const auto it = controllerFactoryMap.find(name);
    assert(it == controllerFactoryMap.end());
    
    controllerFactoryMap[name] = std::move(factory);
}


static ControllerFactoryMap createControllerFactoryMap(Xenobuild::Context &context) {
    ControllerFactoryMap result;
    
    appendFactoryController<Xenobuild::BuildController>(result, context);
    appendFactoryController<Xenobuild::SetupController>(result, context);
    appendFactoryController<Xenobuild::ConfigureController>(result, context);

    return result;
}


namespace Xenobuild {
    class ControllerManager {
    public:
        explicit ControllerManager(Xenobuild::Context &context) {
            factoryMap = createControllerFactoryMap(context);
        }
        
        std::unique_ptr<Controller> create(const std::string &name, std::vector<char*> &args) const {
            const auto factoryIt = factoryMap.find(name);
            
            if (factoryIt == factoryMap.end()) {
                return {};
            }
            
            assert(factoryIt->second);
            
            return factoryIt->second->createController(args.size(), args.data());
        }
        
        std::vector<std::string> enumerate() const {
            std::vector<std::string> names;
            
            names.push_back(Xenobuild::BuildController::Name);
            names.push_back(Xenobuild::SetupController::Name);
            names.push_back(Xenobuild::ConfigureController::Name);
            
            return names;
        }
        
    private:
        ControllerFactoryMap factoryMap;
    };
    
    
    boost::optional<Xenobuild::Package> createPackage(Xenobuild::PackageFactory &factory, const boost::filesystem::path &filePath) {
        std::fstream fs;
        
        fs.open(filePath.string().c_str(), std::ios::in);
        if (! fs.is_open()) {
            return {};
        }
        
        Xenobuild::Package package = factory.createPackage(fs);
        
        if (filePath.has_parent_path()) {
            package.path = filePath.parent_path();
        } else {
            package.path = boost::filesystem::current_path();
        }
        
        return package;
    }
}


int main(int argc, char **argv) {
    Xenobuild::FileSystemPackageFactory packageFactory;
    boost::optional<Xenobuild::Package> package = Xenobuild::createPackage(packageFactory, "Xenobuild.yaml");
    
    if (!package) {
        std::cerr << "There is no accesible Xenobuild.yaml file in the current folder" << std::endl;
        return EXIT_FAILURE;
    }
    
    if (argc < 2) {
        std::cerr << "No subcommand supplied." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<char*> args = {argv + 1, argv + argc};
    const std::string command = args[0];
    
    Xenobuild::ToolchainInstallPathEnumerator toolchainEnumerator;

    const auto toolchainPaths = toolchainEnumerator.enumerate(Xenobuild::ToolchainType::MicrosoftVC);

    Xenobuild::Toolchain toolchain {
        Xenobuild::Triplet{{}, Xenobuild::ToolchainType::MicrosoftVC}, 
        toolchainPaths.size() > 0 ?  toolchainPaths[0] : ""
    };

    Xenobuild::Context context {
        package.get(),
        toolchain
    };

    Xenobuild::ControllerManager manager{context};
    
    auto controller = manager.create(command, args);
    
    if (!controller) {
        std::cerr << "Unknown command " << command << "." << std::endl;
        std::cerr << "Available commands: " << std::endl;
        
        for (const std::string &cmd : manager.enumerate()) {
            std::cerr << "    " << cmd << std::endl;
        }
        
        return EXIT_FAILURE;
    }
    
    controller->perform();
    
    return EXIT_SUCCESS;
}
