
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <optional>

#include <nlohmann/json.hpp>

#include <bok/core/FileService_FS.hpp>
#include <bok/core/pipeline/FileScanner_FS.hpp>

#include <bok/utility/WildcardClassifier.hpp>


namespace bok {
    enum FILE_TYPE {
        FT_CPP_SOURCE,
        FT_JSON
    };

    using FileClassifier = WildcardClassifier<FILE_TYPE>;
}

namespace bok::ir {
    struct ComponentIR {
        std::string name;
        std::string type;
        std::string language;
        std::vector<std::string> sources;
    };

    struct PackageIR {
        std::string name;
        std::vector<ComponentIR> components;
    };

    /**
     * @brief a Parse Intermediate Representation, that maps the current content structure in a multi-file parsing operation
     */
    struct ContextIR {
        PackageIR package;
    };

    class ContextIRLoader {
    public:
        ContextIR parse(const boost::filesystem::path &rootFile) const {
            const std::string content = fileService.load(rootFile.string());

            // nlohmann::to_json()

            return {};
        }

    private:
        FileClassifier classifier;
        FileService_FS fileService;
    };
}


constexpr const char* getPackagePath() {
    return CMAKE_CURRENT_SOURCE_DIR "\\data\\cpp-core\\01-hello-world";
}


int main() {
    bok::FileClassifier classifier{};
    classifier.registerCategory(bok::FT_CPP_SOURCE, {"*.cpp", "*.cc", "*.c++", "*.cxx" });
    classifier.registerCategory(bok::FT_JSON, { "*.json" });

    auto filter = [&classifier](const boost::filesystem::path &path) {
        return classifier.getFileType(path.string()) == bok::FT_CPP_SOURCE;
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
