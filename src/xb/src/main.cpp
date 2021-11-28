
#include "Xenobuild.h"

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


int main(int argc, char **argv) {
    using namespace Xenobuild;

    PackageFactory factory;
    const Package package = factory.createMockPackage();

    return EXIT_SUCCESS;
}
