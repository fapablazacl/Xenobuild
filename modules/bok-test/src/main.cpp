
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <optional>

#include <nlohmann/json.hpp>

#include <bok/core/pipeline/TaskGraphGenerator_Build.hpp>
#include <bok/core/toolchain/Compiler.hpp>
#include <bok/core/toolchain/Linker.hpp>
#include <bok/core/toolchain/Toolchain.hpp>
#include <bok/core/toolchain/ToolchainFactory_Mock.hpp>
#include <bok/core/FileService_FS.hpp>
#include <bok/core/pipeline/FileScanner_FS.hpp>
#include <bok/utility/WildcardClassifier.hpp>

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


constexpr const char* getPackagePath() {
    return CMAKE_CURRENT_SOURCE_DIR "\\data\\cpp-core\\01-hello-world";
}

int main() {
    auto taskGraphGeneratorBuild = bok::TaskGraphGenerator_Build{};

    auto toolchainManager = bok::ToolchainFactory_Mock{};
    auto toolchain = toolchainManager.getToolchain("mock");

    auto compiler = toolchain->enumerateCompilers()[0];
    auto linker = toolchain->enumerateLinkers()[0];
    
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