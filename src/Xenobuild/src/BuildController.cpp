
// BuildController:
// relationship between components and source files
//    1. Read the Package to Build 
//    2. Create the Abstract TaskGraph
//        2.1 Create a AbstractTaskGraph with the dependency relationship between the different Modules in a Package. Collect errors
//        2.2 Create a AbstractTaskGraph, with the dependency relationships between the different SourceFiles in a Modules. Collect errors
//    3. Optimize the Abstract TaskGraph
//    4. Concretize the TaskGraph, with an explicit
//
//Layers:
//    Model Domain
//        Package
//        Toolchain
//
//    PackageGraph
//
//    Build
//        TaskGraph
//
//    Clean
//        TaskGraph


#include <Xenobuild/BuildController.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Version.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageFactory.h>
#include <Xenobuild/core/Module.h>
#include <Xenobuild/core/SourceFile.h>

#include <iostream>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/graph/adjacency_list.hpp>


namespace Xenobuild {
    BuildControllerInput BuildControllerInput::parse(const std::vector<std::string> &args) {
        const auto currentPath = boost::filesystem::current_path();

        BuildControllerInput result;

        result.sourceDir = currentPath.string();
        result.buildDir = (currentPath / ".Xenobuild").string();

        return result;
    }
}


namespace Xenobuild {
    struct ModuleGraph {

    };

    struct PackageGraph {

    };

    struct AbstractCompileCommand {
        boost::filesystem::path sourceFilePath;
        boost::filesystem::path outputFilePath;
    };

    /**
     * @brief Abstract ModuleTaskGraph
     */
    struct ModuleTaskGraph {
        struct VertexData {
            SourceFile sourceFile;
        };

        struct EdgeData {
            std::string label;
            boost::optional<AbstractCompileCommand> command;
        };

        using AdjacencyList = boost::adjacency_list<
            boost::vecS, boost::vecS, boost::directedS, VertexData, EdgeData>;

        size_t moduleVD = 0;
        AdjacencyList adjacencyList;

        /**
         * @brief Creates an abstract TaskGraph to build the specified Module object.
         * 
         * @param module The module to create the TaskGraph for.
         * @return ModuleTaskGraph , ready to be further proccessed 
         */
        static ModuleTaskGraph create(const Module &module) {
            ModuleTaskGraph graph;

            return graph;
        }
    };
}


namespace Xenobuild {
    const char* BuildController::Name = "build";
    
    BuildController::BuildController(Context& context, const BuildControllerInput& params)
        : context(context), params(params) {}

    void BuildController::perform() {
        
    }
}
