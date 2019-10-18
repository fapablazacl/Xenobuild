
#include "BuildController.hpp"

#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/graph/graphviz.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/LanguageEntity.hpp>
#include <borc/entity/ModuleEntity.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Package.hpp>
#include <borc/services/BuildServiceImpl.hpp>
#include <borc/services/LoggingServiceImpl.hpp>
#include <borc/toolchain/ToolchainFactory.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/build/BuildCache.hpp>
#include <borc/build/ConfigurationService.hpp>
#include <borc/utility/DagNode.hpp>
#include <borc/utility/Dag.hpp>
#include <borc/utility/DagVisitor.hpp>

#include "BuildControllerOptions.hpp"
#include <borc/common/EntityLoader.hpp>
#include <borc/common/EntityLoaderFactory.hpp>
#include <borc/common/PackageFactory.hpp>

namespace borc {
    BuildController::~BuildController() {}

    void BuildController::perform(const BuildControllerOptions &options) {
        if (options.showHelp) {
            std::cout << options.helpMessage;
            return;
        }

        // perform the configure operation
        FileServiceImpl service;
        const boost::filesystem::path baseFolderPath = boost::filesystem::current_path();

        EntityLoaderFactory entityLoaderFactory;

        const auto entityLoader = entityLoaderFactory.createLoader(baseFolderPath, service);
        const PackageEntity packageEntity = entityLoader->loadPackageEntity();
        const std::vector<ModuleEntity> moduleEntities = entityLoader->loadModuleEntities(packageEntity);
        
        LoggingServiceImpl loggingService {"BuildServiceImpl"};

        PackageFactory packageFactory;
        std::unique_ptr<Package> package = packageFactory.createPackage(packageEntity, moduleEntities);

        const boost::filesystem::path outputPath = baseFolderPath / ".borc";

        ConfigurationService configurationService {outputPath};
        ConfigurationData configurationData = configurationService.getData();

        if (configurationData.buildConfigurations.size() == 0) {
            throw std::runtime_error("No configurations detected. Please, run 'borc configure --help' for details");
        }

        if (! configurationData.currentBuildConfiguration) {
            std::cout << "No configuration selected as the current one. Picking the first one" << std::endl;
            configurationData.currentBuildConfiguration = *configurationData.buildConfigurations.begin();
        }

        auto buildCache = configurationService.createBuildCache(configurationData.currentBuildConfiguration.get());

        std::cout << "Building configuration " << configurationData.currentBuildConfiguration.get().computeIdentifier() << " ..." << std::endl;

        auto toolchainFactory = ToolchainFactory::create();
        auto toolchain = toolchainFactory->createToolchain(configurationData.currentBuildConfiguration.get().toolchainId);

        BuildServiceImpl buildService {
            baseFolderPath, 
            outputPath / configurationData.currentBuildConfiguration.get().computeIdentifier(), 
            toolchain.get(), 
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
