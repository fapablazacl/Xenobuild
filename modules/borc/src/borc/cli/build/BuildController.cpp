
#include "BuildController.hpp"

#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/graph/graphviz.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/entity/PackageEntity.hpp>
#include <borc/entity/LanguageEntity.hpp>
#include <borc/entity/ModuleEntity.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/model/Package.hpp>
#include <borc/services/BuildServiceImpl.hpp>
#include <borc/services/LoggingServiceImpl.hpp>
#include <borc/toolchain/ToolchainFactory.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/build/BuildCache.hpp>
#include <borc/build/BuildCacheFactory.hpp>
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

        // TODO: construct the output folder based on the current toolchain and version.
        // TODO: Create the BuildCache from a factory, because it depends on the current toolchain.
        const boost::filesystem::path outputPath = baseFolderPath / ".borc";

        BuildCacheFactory buildCacheFactory;

        BuildCache *buildCache = buildCacheFactory.createBuildCache(outputPath);

        BuildCacheData buildCacheData = buildCache->getData();

        if (buildCacheData.buildConfigurations.size() == 0) {
            throw std::runtime_error("No configurations detected. Please, run 'borc configure --help' for details");
        }

        if (! buildCacheData.currentBuildConfiguration) {
            std::cout << "No configuration selected as the current one. Picking the first one" << std::endl;
            buildCacheData.currentBuildConfiguration = *buildCacheData.buildConfigurations.begin();
        }

        std::cout << "Building configuration " << buildCacheData.currentBuildConfiguration.get().computeIdentifier() << " ..." << std::endl;

        auto toolchainFactory = ToolchainFactory::create();
        auto toolchain = toolchainFactory->createToolchain(buildCacheData.currentBuildConfiguration.get().toolchainId);

        BuildServiceImpl buildService {
            baseFolderPath, 
            outputPath / buildCacheData.currentBuildConfiguration.get().computeIdentifier(), 
            toolchain.get(), 
            buildCache, 
            &loggingService
        };

        /*
        std::cout << "Computing source dependencies for package '" << package->getName() << "' ..." << std::endl;

        if (package->getArtifacts().size() == 0) {
            throw std::runtime_error("No artifacts detected for package '" + package->getName() + "'.");
        }

        auto dependencyGraph = buildService.computeDependencyGraph(package->getArtifacts()[0]);
        
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
