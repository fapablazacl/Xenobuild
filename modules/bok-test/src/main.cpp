
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
#include <boost/algorithm/string.hpp>

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
        std::vector<std::string> stdoutStreamOutput;
        std::vector<std::string> stderrStreamOutput;
    };

    class CommandExecutor {
    public:
        virtual ~CommandExecutor() {}

        virtual CommandResult execute(const CommandData& commandData) const = 0;
    };

    class CommandExecutor_BoostProcess : public CommandExecutor {
    public:
        CommandResult execute(const CommandData& commandData) const override {
            boost::filesystem::path compilerPath = commandData.path;

            if (commandData.path == "") {
                compilerPath = boost::process::search_path(commandData.name);
            } else {
                compilerPath /= commandData.name;
            }

            boost::process::ipstream stdoutStream;
            boost::process::ipstream stderrStream;

            boost::process::child childProcess {
                compilerPath, 
                boost::process::args(commandData.args), 
                boost::process::std_out > stdoutStream,
                boost::process::std_err > stderrStream,
            };

            const std::vector<std::string> stdoutStreamOutput = grabStreamOutput(stdoutStream);
            const std::vector<std::string> stderrStreamOutput = grabStreamOutput(stderrStream);

            childProcess.wait();

            return {
                childProcess.exit_code(),
                stdoutStreamOutput,
                stderrStreamOutput
            };
        }

    private:
        std::vector<std::string> grabStreamOutput(boost::process::ipstream &stream) const {
            std::string line;
            std::vector<std::string> lines;

            while (stream && std::getline(stream, line) && !line.empty()) {
                lines.push_back(line);
            }

            return lines;
        }
    };

    /**
     * @brief Perform an operation for each edge in the graph
     */
    class TaskGraphVisitor {
    public:
        void visit(const TaskGraph& graph) const {
            visit(graph.moduleVertexDescriptor, graph.adjacencyList);
        }

    private:
        void visit(const size_t vd, const TaskGraphAdjacencyList &al) const {
            for (auto cvd : boost::make_iterator_range(boost::adjacent_vertices(vd, al))) {
                visit(cvd, al);
            }

            for (auto ed : boost::make_iterator_range(boost::out_edges(vd, al))) {
                if (!al[ed].commandRequired) {
                    continue;
                }

                std::cout << al[ed].label << std::endl;

                const auto result = executor.execute(al[ed].command);

                if (result.stderrStreamOutput.size() != 0) {
                    std::cout << al[ed].command << std::endl << std::endl;
                    std::cout << "failed:" << std::endl;

                    for (const auto& line : result.stderrStreamOutput) {
                        std::cout << line << std::endl;
                    }
                }
            }
        }

        void showDebugDiagnostics(const TaskGraph& graph) const {
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
        CommandExecutor_BoostProcess executor;
    };
}


int main() {
    auto classifier = createSourceClassifier();

    auto package = createPackage_01HelloWorld();
    // auto package = createPackage_02WordCounter();
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
