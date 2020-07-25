
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <optional>

#include <nlohmann/json.hpp>

#include <bok/core/FileService_FS.hpp>
#include <bok/core/pipeline/DiskFileScanner.hpp>
#include <bok/utility/string.hpp>


namespace bok {
    enum FILE_TYPE {
        FT_UNKNOWN , 
        FT_CPP_SOURCE,
        FT_JSON
    };

    class FileClassifier {
    public:
        FileClassifier() {
            typeWildCardMap[FT_CPP_SOURCE] = {
                "*.cpp", "*.cc", "*.c++", "*.cxx"
            };

            typeWildCardMap[FT_JSON] = {
                "*.json"
            };
        }

        FILE_TYPE getFileType(const boost::filesystem::path &path) const {
            const auto file = path.filename().string();

            for (const auto &keyValue : typeWildCardMap) {
                for (const std::string &pattern : keyValue.second) {
                    if (wildcard_match(pattern.c_str(), file.c_str())) {
                        return keyValue.first;
                    }
                }
            }

            return FT_UNKNOWN;
        }

    private:
        std::map<FILE_TYPE, std::vector<std::string>> typeWildCardMap;
    };
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
            assert(classifier.getFileType(rootFile) == FT_JSON);

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

    auto filter = [&classifier](const boost::filesystem::path &path) {
        return classifier.getFileType(path) == bok::FT_CPP_SOURCE;
    };

    const std::string packagePath = getPackagePath();
    const std::vector<boost::filesystem::path> files = bok::DiskFileScanner{}.scan(packagePath, bok::FileScanner::Recursive, filter);

    for (const auto &file : files) {
        std::cout << file << std::endl;
    }

    return 0;
}
