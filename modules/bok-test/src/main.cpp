
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

namespace bok {
    enum FILE_TYPE {
        FT_CPP_SOURCE,
        FT_JSON
    };

    class FileClassifier : public WildcardClassifier<FILE_TYPE> {
    public:
        FileClassifier() {
            registerCategory(FT_CPP_SOURCE, { "*.cpp", "*.cc", "*.c++", "*.cxx" });
            registerCategory(FT_JSON, { "*.json" });
        }
    };
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


namespace bok {
    /**
     * @brief Perform an operation for each edge in the graph
     */
    class TaskGraphVisitor {
    public:
        void visit(const TaskGraph& graph) const {
            // boost::adjacent_vertices()
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

                std::cout << " label: " << al[ed].label << std::endl;
                std::cout << " command: " << al[ed].command << std::endl;
                std::cout << std::endl;
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
    };
}


int main() {
    auto package = createPackage_01HelloWorld();
    auto taskGraphGeneratorBuild = bok::TaskGraphGenerator_Build{};
    auto toolchainManager = bok::ToolchainFactory_Mock{};
    auto toolchain = toolchainManager.getToolchain("mock");
    auto visitor = bok::TaskGraphVisitor{};

    visitor.visit(taskGraphGeneratorBuild.generate(toolchain, package->getModules()[0]));

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
