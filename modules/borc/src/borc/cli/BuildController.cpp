
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
#include <borc/build/BuildCacheFactory.hpp>
#include <borc/utility/DagNode.hpp>
#include <borc/utility/Dag.hpp>
#include <borc/utility/DagVisitor.hpp>

#include "BuildControllerOptions.hpp"
#include "../common/EntityLoader.hpp"
#include "../common/EntityLoaderFactory.hpp"
#include "../common/PackageFactory.hpp"

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
        
        // we have all the information needed in order to process the packages.
        /*
        std::set<std::string> languages;

        for (const ModuleEntity &moduleEntity : moduleEntities) {
            languages.insert(moduleEntity.language);
        }

        std::cout << "Detected programming languages:" << std::endl;
        for (const std::string &lang : languages) {
            std::cout << "    " << lang << std::endl;
        }

        // with the programming languages, we can issue to the user what toolchains must be configured!
        std::map<std::string, std::string> toolchains = {
            {"c++/17", "gcc"},
            {"c++/17", "clang"}
        };
        */

        LoggingServiceImpl loggingService {"BuildServiceImpl"};

        auto toolchainFactory = ToolchainFactory::create();
        auto toolchain =toolchainFactory->createToolchain(ToolchainFamily::GCC);

        PackageFactory packageFactory;
        std::unique_ptr<Package> package = packageFactory.createPackage(packageEntity, moduleEntities);

        // TODO: construct the output folder based on the current toolchain and version.
        // TODO: Create the BuildCache from a factory, because it depends on the current toolchain.
        const boost::filesystem::path outputPath = baseFolderPath / ".borc" / "gcc";

        BuildCacheFactory buildCacheFactory;

        BuildCache *buildCache = nullptr;
        if (options.force) {
            buildCache = buildCacheFactory.createNullBuildCache();
        } else {
            buildCache = buildCacheFactory.createBuildCache(outputPath);
        }

        BuildServiceImpl buildService{baseFolderPath, outputPath, toolchain.get(), buildCache, &loggingService};

        std::cout << "Computing source dependencies for package '" << package->getName() << "' ..." << std::endl;

        auto dependencyGraph = buildService.computeDependencyGraph(package->getArtifacts()[0]);

        std::fstream fs;
        fs.open("output.dot", std::ios_base::out);
        boost::write_graphviz(fs, dependencyGraph);

        /*
        auto dag = buildService.createBuildDag(package.get());
        DagVisitor dagVisitor;
        dagVisitor.visit(dag.get());
        */

        // Now we need to start the build!
        // Now we have parsed all the artifacts in the main package. 
        // Let's parse all the additional packages. We need that when we solve all the dependencies to create references
        /*
        std::vector<std::unique_ptr<Package>> packages;

        for (const std::string &packageSearchPathStr : packageEntity.packageSearchPaths) {
            // TODO: Add package parsing routine
        }
        */
    }
}
