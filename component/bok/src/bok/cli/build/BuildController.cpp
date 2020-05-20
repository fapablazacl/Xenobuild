
#include "BuildController.hpp"

#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/graph/graphviz.hpp>

#include <bok/core/common/Constants.hpp>
#include <bok/core/common/FileServiceImpl.hpp>
#include <bok/core/pipeline/BuildServiceImpl.hpp>
#include <bok/core/package/PackageServiceImpl.hpp>
#include <bok/core/package/Module.hpp>
#include <bok/core/package/Package.hpp>
#include <bok/core/package/PackageRegistry.hpp>
#include <bok/core/package/PackageRegistryFactory.hpp>
#include <bok/core/common/LoggingServiceImpl.hpp>
#include <bok/core/toolchain/ToolchainFactoryFS.hpp>
#include <bok/core/toolchain/Toolchain.hpp>

#include <bok/feature/build/BuildCache.hpp>
#include <bok/feature/build/ConfigurationService.hpp>


#include <bok/utility/DagNode.hpp>
#include <bok/utility/Dag.hpp>
#include <bok/utility/DagVisitor.hpp>

#include "BuildControllerOptions.hpp"

namespace bok {
    BuildController::~BuildController() {}


    void BuildController::perform(const BuildControllerOptions &options) {
        if (options.showHelp) {
            std::cout << options.helpMessage;
            return;
        }

        const boost::filesystem::path baseFolderPath = options.sourcePath 
            ? options.sourcePath.get()
            : boost::filesystem::current_path();

        const boost::filesystem::path outputPath = options.outputPath
            ? options.outputPath.get()
            : baseFolderPath / ".bok";

        const FileServiceImpl fileService;

        auto packageService = std::make_unique<PackageServiceImpl>(&fileService);
        auto packageRegistryFactory = std::make_unique<PackageRegistryFactory>();
        auto packageRegistry = packageRegistryFactory->createPackageRegistry(packageService.get(), BOK_PACKAGE_SEARCH_PATH);
        auto package = packageService->createPackage(baseFolderPath, packageRegistry.get());

        LoggingServiceImpl loggingService {"BuildServiceImpl"};

        ConfigurationService configurationService {outputPath, baseFolderPath};
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

        auto toolchainFactory = std::make_unique<ToolchainFactoryFS>("./toolchain/", boost::filesystem::path{configurationData.currentBuildConfiguration->toolchainPath});

        auto toolchain = toolchainFactory->createToolchain(configurationData.currentBuildConfiguration.get().toolchainId);

        BuildServiceImpl buildService {
            baseFolderPath, 
            outputPath / configurationData.currentBuildConfiguration.get().computeIdentifier(), 
            toolchain, 
            buildCache.get(), 
            &loggingService
        };

        /*
        std::cout << "Computing source dependencies for package '" << package->getName() << "' ..." << std::endl;

        if (package->getModules().size() == 0) {
            throw std::runtime_error("No modules detected for package '" + package->getName() + "'.");
        }

        auto dependencyGraph = buildService.computeDependencyGraph(package->getModules()[0]);
        
        std::fstream fs;
        fs.open("output.dot", std::ios_base::out);
        boost::write_graphviz(fs, dependencyGraph,
            boost::make_label_writer(boost::get(&DependencyGraphVertexData::label, dependencyGraph)),
            boost::make_label_writer(boost::get(&DependencyGraphEdgeData::label, dependencyGraph))
        );
        */

        auto dag = buildService.createBuildDag(package.get());
        DagVisitor dagVisitor;
        dagVisitor.visit(dag.get());
    }
}
