
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <memory>
#include <sstream>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

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
    struct CliSubcommandDesc {
        std::string name;
        std::string description;
    };

    class ControllerManager {
    public:
        explicit ControllerManager(Xenobuild::Context &context) {
            factoryMap = createControllerFactoryMap(context);
        }
        
        std::unique_ptr<Controller> create(const std::string &name, std::vector<std::string> &args) const {
            const auto factoryIt = factoryMap.find(name);
            
            if (factoryIt == factoryMap.end()) {
                return {};
            }
            
            assert(factoryIt->second);
            
            return factoryIt->second->createController(args);
        }
        
        std::vector<CliSubcommandDesc> enumerate() const {
            std::vector<CliSubcommandDesc> subcommands;
            
            subcommands.push_back(CliSubcommandDesc{Xenobuild::BuildController::Name, "This subcommand performs something"});
            subcommands.push_back(CliSubcommandDesc{Xenobuild::SetupController::Name, "This subcommand performs something"});
            subcommands.push_back(CliSubcommandDesc{Xenobuild::ConfigureController::Name, "This subcommand performs something"});
            
            return subcommands;
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


std::tuple<
    boost::program_options::parsed_options, 
    boost::program_options::variables_map> parseGlobalOptions(int argc, char** argv) {

    boost::program_options::options_description global("Global options");
    global.add_options()
        ("debug", "Turn on debug output")
        ("command", boost::program_options::value<std::string>(), "command to execute")
        ("subargs", boost::program_options::value<std::vector<std::string> >(), "Arguments for command");

    boost::program_options::positional_options_description pos;
    pos.add("command", 1).add("subargs", -1);

    boost::program_options::variables_map vm;

    boost::program_options::parsed_options parsed = boost::program_options::command_line_parser(argc, argv).
        options(global).
        positional(pos).
        allow_unregistered().
        run();

    boost::program_options::store(parsed, vm);

    return {parsed, vm};
}


std::string generateHelpString(const std::vector<Xenobuild::CliSubcommandDesc> &subcommands) {
    std::stringstream ss;

    ss << "Available subcommands: " << std::endl;

    for (const Xenobuild::CliSubcommandDesc &subcommand : subcommands) {
        ss << "    " << subcommand.name << " -- " << subcommand.description << std::endl;
    }

    return ss.str();
}


int main(int argc, char **argv) {
    try {
        Xenobuild::FileSystemPackageFactory packageFactory;
        boost::optional<Xenobuild::Package> package = Xenobuild::createPackage(packageFactory, "Xenobuild.yaml");
        
        if (!package) {
            throw std::runtime_error("There is no accesible Xenobuild.yaml file in the current folder");
        }
    
        const Xenobuild::Triplet triplet = Xenobuild::Triplet::createDefaultTriplet();
        const Xenobuild::ToolchainInstallPathEnumerator toolchainEnumerator;
        const std::vector<std::string> toolchainPaths = toolchainEnumerator.enumerate(triplet.toolchainType);
        
        Xenobuild::Toolchain toolchain {triplet,
            toolchainPaths.size() > 0 ?  toolchainPaths[0] : ""
        };

        Xenobuild::Context context {
            package.get(),
            toolchain
        };

        Xenobuild::ControllerManager manager{context};
    
        const auto [parsed, vm] = parseGlobalOptions(argc, argv);

        if (vm["command"].empty()) {
            throw std::runtime_error("No subcommand supplied\n" + generateHelpString(manager.enumerate()));
        }

        const std::string command = vm["command"].as<std::string>();
        
        std::vector<std::string> args = boost::program_options::collect_unrecognized(parsed.options,  boost::program_options::include_positional);
        args.erase(args.begin());

        auto controller = manager.create(command, args);
    
        if (!controller) {
            std::stringstream ss;
            ss << "Unknown command " << command << "." << std::endl;
            
            throw std::runtime_error(ss.str() + generateHelpString(manager.enumerate()));
        }
    
        controller->perform();
    
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exp) {
        std::cerr << exp.what() << std::endl;

        return EXIT_FAILURE;
    }
}
