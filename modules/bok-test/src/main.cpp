
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <optional>

#include <nlohmann/json.hpp>

#include <bok/core/package/Package.hpp>
#include <bok/core/package/Module.hpp>
#include <bok/core/package/Source.hpp>
#include <bok/core/pipeline/TaskGraphGenerator_Build.hpp>
#include <bok/core/toolchain/Compiler.hpp>
#include <bok/core/toolchain/Linker.hpp>
#include <bok/core/toolchain/Toolchain.hpp>
#include <bok/core/toolchain/ToolchainFactory_Mock.hpp>
#include <bok/core/FileService_FS.hpp>
#include <bok/core/pipeline/FileScanner_FS.hpp>
#include <bok/utility/WildcardClassifier.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/process.hpp>

bok::WildcardClassifier<bok::CompilerType> createSourceClassifier() {
    bok::WildcardClassifier<bok::CompilerType> classifier;

    classifier.registerCategory(bok::CompilerType::Source, { "*.cpp", "*.cc", "*.c++", "*.cxx" });

    return classifier;
}


std::unique_ptr<bok::Package> createPackage_01HelloWorld() {
    const std::string path = CMAKE_CURRENT_SOURCE_DIR "\\data\\cpp-core\\01-hello-world";

    auto package = std::make_unique<bok::Package>("01-hello-world");

    auto module = package->createModule<bok::Module>();
    module->setName("01-hello-world");
    module->setPath(path);
    module->setType(bok::Module::Type{ "app", "cli" });
    module->setSourcePaths({"main.cpp"});
    module->rescanSources("");

    return package;
}


std::unique_ptr<bok::Package> createPackage_02WordCounter() {
    const std::string path = CMAKE_CURRENT_SOURCE_DIR "\\data\\cpp-core\\02-word-counter";

    auto package = std::make_unique<bok::Package>("02-word-counter");
    auto module = package->createModule<bok::Module>();
    module->setName("02-word-counter");
    module->setPath(path);
    module->setType(bok::Module::Type{ "app", "cli" });
    module->setSourcePaths({
        "main.cpp",
        "WordCounter.hpp",
        "WordCounter.cpp",
        "WordList.hpp",
        "WordList.cpp"
    });

    module->rescanSources("");

    return package;
}

namespace bok {
    struct CommandResult {
        int exitCode;
        std::vector<std::string> output;
    };

    class CommandExecutor {
    public:
        CommandResult execute(const CommandData& commandData) const {
            boost::filesystem::path compilerPath = commandData.path;

            if (commandData.path == "") {
                compilerPath = boost::process::search_path(commandData.name);
            } else {
                compilerPath /= commandData.name;
            }

            boost::process::ipstream pipeStream;
            boost::process::child childProcess {
                compilerPath, 
                boost::process::args(commandData.args), 
                boost::process::std_out > pipeStream
            };

            std::string line;
            std::vector<std::string> specs;

            while (pipeStream && std::getline(pipeStream, line) && !line.empty()) {
                specs.push_back(line);
            }

            childProcess.wait();

            return {
                childProcess.exit_code(),
                specs
            };
        }
    };


    /**
     * @brief Perform an operation for each edge in the graph
     */
    class TaskGraphVisitor {
    public:
        void visit(const TaskGraph& graph) const {
            // showDiagnostics(graph);

            visit(graph.moduleVertexDescriptor, graph.adjacencyList);
        }

    private:
        void visit(const size_t vd, const TaskGraphAdjacencyList &al) const {
            for (auto cvd : boost::make_iterator_range(boost::adjacent_vertices(vd, al))) {
                visit(cvd, al);

                // std::cout << " label: " << al[cvd].label << std::endl;
                // std::cout << " filePath: " << boost::filesystem::path(al[cvd].filePath).filename() << std::endl;
                // std::cout << std::endl;
            }

            for (auto ed : boost::make_iterator_range(boost::out_edges(vd, al))) {
                if (!al[ed].commandRequired) {
                    continue;
                }

                // std::cout << " label: " << al[ed].label << std::endl;
                // std::cout << " command: " << al[ed].command << std::endl;

                const auto result = executor.execute(al[ed].command);

                if (result.exitCode != 0) {
                    std::cout << "OK" << std::endl;
                } else {
                    std::cout << "Error" << std::endl;
                }

                for (const auto& line : result.output) {
                    std::cout << line << std::endl;
                }
            }
        }

        void showDiagnostics(const TaskGraph& graph) const {
            std::cout << "Number of Vertices: " << boost::num_vertices(graph.adjacencyList) << std::endl;
            std::cout << "Number of Edges:    " << boost::num_edges(graph.adjacencyList) << std::endl;
            std::cout << "Detail of Vertices:" << std::endl;

            for (auto vd : boost::make_iterator_range(boost::vertices(graph.adjacencyList))) {
                std::cout << " label: " << graph.adjacencyList[vd].label << std::endl;
                std::cout << " filePath: " << boost::filesystem::path(graph.adjacencyList[vd].filePath).filename() << std::endl;
                std::cout << std::endl;
            }

            for (auto ed : boost::make_iterator_range(boost::edges(graph.adjacencyList))) {
                std::cout << " label: " << graph.adjacencyList[ed].label << std::endl;
                std::cout << " command: " << graph.adjacencyList[ed].command << std::endl;
                std::cout << std::endl;
            }
        }

    private:
        CommandExecutor executor;
    };
}


int main() {
    // C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.22.27905\bin\Hostx64\x64

    auto classifier = createSourceClassifier();

    // auto package = createPackage_01HelloWorld();
    auto package = createPackage_02WordCounter();
    auto taskGraphGeneratorBuild = bok::TaskGraphGenerator_Build{classifier};
    auto toolchainManager = bok::ToolchainFactory_Mock{};
    // auto toolchain = toolchainManager.getToolchain("mock");
    auto toolchain = toolchainManager.getToolchain("vc");
    auto visitor = bok::TaskGraphVisitor{};

    auto graph = taskGraphGeneratorBuild.generate(toolchain, package->getModules()[0]);

    visitor.visit(graph);

    return 0;
}

/*
int main() {
    auto classifier = bok::FileClassifier{};
    
    auto filter = [&classifier](const boost::filesystem::path &path) {
        const auto fileType = classifier.getFileType(path.string());

        return fileType.has_value() && fileType == bok::FT_CPP_SOURCE;
    };

    const std::string packagePath = getPackagePath();
    const std::vector<boost::filesystem::path> files = bok::FileScanner_FS{}.scan(packagePath, bok::FileScanner::Recursive, filter);

    if (files.empty()) {
        std::cout << "No files where found in path '" << getPackagePath() << "'" << std::endl;
    } else {
        for (const auto& file : files) {
            std::cout << file << std::endl;
        }
    }

    return 0;
}
*/
