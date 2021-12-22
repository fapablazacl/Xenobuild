
#pragma once 


#include <iostream>
#include <vector>
#include <map>
#include <xb/core/Command.h>
#include <xb/core/Version.h>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/graph/adjacency_list.hpp>

// core-cpp:
// relationship between components and source files

/*
    1. Read the Package to Build 
    2. Create the Abstract TaskGraph
        2.1 Create a AbstractTaskGraph with the dependency relationship between the different Modules in a Package. Collect errors
        2.2 Create a AbstractTaskGraph, with the dependency relationships between the different SourceFiles in a Modules. Collect errors
    3. Optimize the Abstract TaskGraph
    4. Concretize the TaskGraph, with an explicit

Layers:
    Model Domain
        Package
        Toolchain

    PackageGraph

    Build
        TaskGraph

    Clean
        TaskGraph
*/


// Model Domain Layer 
namespace Xenobuild {
    struct SourceFileType {

    };

    struct Rule {
        SourceFileType in;
        SourceFileType out;
    };

    //! Pipeline
    struct Pipeline {

    };
}


namespace Xenobuild {
    struct SourcePath {
        boost::filesystem::path path;
    };

    struct Module {
        std::string name;
        std::string type;
        std::string lang;
        boost::filesystem::path path;
        std::vector<std::string> dependencies;

        //! Paths and Files, where collect we can 
        std::vector<SourcePath> sourcePaths;
    };

    struct Package {
        std::string name;
        boost::filesystem::path path;
        std::vector<Module> modules;
    };
}


namespace Xenobuild::cpp {
    struct SourceFile {
        boost::filesystem::path path;
    };

    enum class ModuleType {
        Executable,
        Library
    };

    struct Module {
        ModuleType type = ModuleType::Executable;
        std::string name;
        boost::filesystem::path path;
        std::vector<std::string> dependencies;
        std::vector<SourceFile> sourceFiles;
    };

    struct Package {
        std::string name;
        boost::filesystem::path path;
        std::vector<Module> modules;
    };
}


namespace Xenobuild {
    struct Version {

    };

    struct Linker {

    };

    struct Compiler {

    };

    struct Toolchain {

    };
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
            std::optional<AbstractCompileCommand> command;
        };

        using AdjacencyList = boost::adjacency_list<
            boost::vecS, boost::vecS, boost::directedS, VertexData, EdgeData
        >;

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
    class PackageFactory {
    public:
        Package createMockPackage() {
            Package package {
                "Test01",
                "borc-old/data/samples/MyApp",
                {
                    { ModuleType::Executable, "MyApp", "MyApp", {"MyLib01", "MyLib02"}, { {"MyApp.cpp"} } },
                    { ModuleType::Library, "MyLib01", "MyLib01", {}, { {"MyLib01.hpp"}, {"MyLib01.cpp"} } },
                    { ModuleType::Library, "MyLib02", "MyLib02", {}, { {"MyLib02.hpp"}, {"MyLib02.cpp"} } }
                }
            };

            checkIntegrity(package);

            return package;
        }

    private:
        void checkIntegrity(const Package &package) {
            std::cout << "Package " << package.name << std::endl;

            for (const Module &module : package.modules) {
                std::cout << "    Module " << module.name << std::endl;

                for (const SourceFile &source : module.sourceFiles) {
                    const auto fullFilePath = package.path / module.path / source.path;

                    if (! boost::filesystem::exists(fullFilePath)) {
                        std::cout << "        SourceFile " << fullFilePath << " doesn't exists." << std::endl;
                    } else  if (! boost::filesystem::is_regular_file(fullFilePath)) {
                        std::cout << "        SourceFile " << fullFilePath << " isn't a regular file." << std::endl;
                    } else {
                        std::cout << "        SourceFile " << source.path << " OK." << std::endl;
                    }
                }
            }
        }
    };
}

using namespace Xenobuild;

static void print(const Package &package) {
    std::cout << "Package " << package.name << std::endl;
    for (const Module &module : package.modules) {
        std::cout << "    Module " << module.name << std::endl;

        for (const SourceFile &source : module.sourceFiles) {
            std::cout << "        SourceFile " << source.path << std::endl;
        }
    }
}

