
#include "BuildController.hpp"

#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/optional.hpp>

#include <bok/core/DagNode.hpp>
#include <bok/core/Dag.hpp>
#include <bok/core/DagVisitor.hpp>
#include <bok/core/Constants.hpp>
#include <bok/core/FileService_FS.hpp>
#include <bok/core/Logger_Console.hpp>
#include <bok/core/pipeline/TaskGraphGenerator_Build.hpp>
#include <bok/core/pipeline/BuildCache.hpp>
#include <bok/core/package/PackageFactory_FS.hpp>
#include <bok/core/package/Module.hpp>
#include <bok/core/package/Package.hpp>
#include <bok/core/package/PackageRegistry.hpp>
#include <bok/core/package/PackageRegistryFactory.hpp>
#include <bok/core/toolchain/ToolchainFactory_FS.hpp>
#include <bok/core/toolchain/Toolchain.hpp>
#include <bok/feature/build/ConfigurationService.hpp>

namespace bok {
    struct BuildControllerOptions {
        bool force = false;
        bool showHelp = false;
        std::string helpMessage;

        boost::optional<boost::filesystem::path> sourcePath;
        boost::optional<boost::filesystem::path> outputPath;

        boost::filesystem::path getRootPath() const {
            return sourcePath 
                ? sourcePath.get()
                : boost::filesystem::current_path();
        }

        boost::filesystem::path getOutputPath() const {
            return outputPath
                ? outputPath.get()
                : this->getRootPath() / ".bok";
        }
    };


    static BuildControllerOptions parse(int argc, char **argv) {
        boost::program_options::options_description desc("Allowed options for Configure subcommand");

        desc.add_options()
            ("help", "produce help message")
            ("force,f", "Force a rebuild")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        BuildControllerOptions options;

        if (vm.count("help")) {
            std::stringstream ss;

            ss << desc << "\n";

            options.showHelp = true;
            options.helpMessage = ss.str();
        }
        
        if (vm.count("force")) {
            options.force = true;
        }

        return options;
    }


    BuildController::BuildController() {
        logger = new Logger_Console();
        fileService = new FileService_FS();
        packageFactory = new PackageFactory_FS(fileService);
        packageRegistryFactory = new PackageRegistryFactory();
        packageRegistry = packageRegistryFactory->createPackageRegistry(packageFactory, BOK_PACKAGE_SEARCH_PATH);
    }


    BuildController::~BuildController() {
        //NOTE: calls smart ptr's destructors ...
        delete packageRegistryFactory;
        delete packageFactory;
        delete fileService;
        delete logger;
    }

    void BuildController::perform(int argc, char **argv) {
        const auto options = parse(argc, argv);

        if (options.showHelp) {
            std::cout << options.helpMessage;
            return;
        }

        const auto rootPath = options.getRootPath();
        const auto outputPath = options.getOutputPath();

        auto package = packageFactory->createPackage(rootPath, packageRegistry.get());

        ConfigurationService configurationService {outputPath, rootPath};
        ConfigurationData configurationData = configurationService.getData();

        if (configurationData.buildConfigurations.size() == 0) {
            throw std::runtime_error("No configurations detected. Please, run 'bok configure --help' for details");
        }

        if (! configurationData.currentBuildConfiguration) {
            std::cout << "No configuration selected as the current one. Picking the first one" << std::endl;
            configurationData.currentBuildConfiguration = *configurationData.buildConfigurations.begin();
        }

        auto buildCache = configurationService.createBuildCache(configurationData.currentBuildConfiguration.get());

        std::cout << "Building configuration " << configurationData.currentBuildConfiguration.get().computeIdentifier() << " ..." << std::endl;

        auto toolchainFactory = std::make_unique<ToolchainFactory_FS>("./toolchain/", boost::filesystem::path{configurationData.currentBuildConfiguration->toolchainPath});
        auto toolchain = toolchainFactory->getToolchain(configurationData.currentBuildConfiguration.get().toolchainId);

        TaskGraphGenerator_Build buildTaskGraphGenerator {
            /*
            rootPath, 
            outputPath / configurationData.currentBuildConfiguration.get().computeIdentifier(), 
            toolchain, 
            buildCache.get(),
            logger
            */
        };

        /*
        std::cout << "Computing source dependencies for package '" << package->getName() << "' ..." << std::endl;

        if (package->getModules().size() == 0) {
            throw std::runtime_error("No modules detected for package '" + package->getName() + "'.");
        }

        auto dependencyGraph = buildTaskGraphGenerator.generate(package->getModules()[0]);
        
        std::fstream fs;
        fs.open("output.dot", std::ios_base::out);
        boost::write_graphviz(fs, dependencyGraph,
            boost::make_label_writer(boost::get(&TaskGraphVertexData::label, dependencyGraph)),
            boost::make_label_writer(boost::get(&TaskGraphEdgeData::label, dependencyGraph))
        );
        */

        /*
        auto dag = buildTaskGraphGenerator.createBuildDag(package.get());
        DagVisitor dagVisitor;
        dagVisitor.visit(dag.get());
        */
    }
}
