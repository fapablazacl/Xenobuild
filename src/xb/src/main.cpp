
#include <iostream>
#include <vector>
#include <xb/core/Command.h>
#include <boost/filesystem.hpp>

// core-cpp:
// relationship between components and source files


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
    std::vector<SourceFile> sourceFiles;
};


struct Package {
    std::string name;
    boost::filesystem::path path;
    std::vector<Module> modules;
};


struct BuildConfig {

};


int main(int argc, char **argv) {
    Package package {
        "Test01",
        "borc-old/data/samples/ConsoleApp01",
        {
            { ModuleType::Executable, "Test01", "./", { {"ConsoleApp01.hpp"}, {"ConsoleApp01.cpp"} } }
        }
    };

    std::cout << "Package " << package.name << std::endl;
    for (const Module &module : package.modules) {
        std::cout << "    Module " << module.name << std::endl;

        for (const SourceFile &source : module.sourceFiles) {
            std::cout << "        SourceFile " << source.path << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
