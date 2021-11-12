
#pragma once 

#include <vector>
#include <string>
#include <optional>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Compiler;
    class Linker;

    struct Version;

    /**
     * @brief An ID to a Compiler Type. Depends on the language and the specific toolchain.
     */
    enum class CompilerType {
        Source,
        // Windows_Resource
    };

    /**
     * @brief An ID to a Linker Type. Depends on the language and the specific toolchain.
     */
    enum class LinkerType {
        Binary,
        // Archiver
    };

    struct PrebuiltPackageBuildData {
        std::vector<boost::filesystem::path> binaryPaths;
        std::vector<boost::filesystem::path> includePaths;
        std::vector<boost::filesystem::path> libraryPaths;
        std::vector<std::string> libraries;

        PrebuiltPackageBuildData merge(PrebuiltPackageBuildData data) const {
            data.binaryPaths.insert(data.binaryPaths.end(), binaryPaths.begin(), binaryPaths.end());
            data.includePaths.insert(data.includePaths.end(), includePaths.begin(), includePaths.end());
            data.libraryPaths.insert(data.libraryPaths.end(), libraryPaths.begin(), libraryPaths.end());
            data.libraries.insert(data.libraries.end(), libraries.begin(), libraries.end());

            return data;
        }
    };

    class PrebuiltPackage {
    public:
        virtual ~PrebuiltPackage() {}

        virtual std::string getName() const = 0;

        virtual PrebuiltPackageBuildData getBuildData() const = 0;

        // virtual BuildData getData(const std::string &moduleName) const = 0;

        // virtual std::vector<std::string> enumerateModules() const = 0;
    };

    class Toolchain {
    public:
        virtual ~Toolchain();

        virtual std::vector<Compiler*> enumerateCompilers() const = 0;

        virtual std::vector<Linker*> enumerateLinkers() const = 0;

        virtual Version detectVersion() const = 0;

        virtual Compiler* getCompiler(const CompilerType type) const = 0;

        virtual Linker* getLinker(const LinkerType type) const = 0;

        /**
         * @brief Get the build information for a package supplied by the toolchain itself
         */
        virtual std::optional<PrebuiltPackageBuildData> getPackageBuildData(const std::string &packageName) const = 0;
    };
}
