
#pragma once 

#include <Xenobuild/core/FileSystemPackageFactory.h>

#include <cassert>
#include <boost/filesystem.hpp>


namespace Xenobuild {
    static bool isCppExt(const std::string& ext) {
        return ext == ".cpp" || ext == ".h" || ext == ".c" || ext == ".hpp";
    }


    static void checkIntegrity(const Package &package) {
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


    Package FileSystemPackageFactory::createMockPackage() {
        Package package {
            "Test01",
            boost::filesystem::path{"borc-old/data/samples/MyApp"},
            {
                Module{ "MyApp", ModuleType::Executable, "MyApp", {"MyLib01", "MyLib02"}, { {"MyApp.cpp"} } },
                Module{ "MyLib01", ModuleType::Library, "MyLib01", {}, { {"MyLib01.hpp"}, {"MyLib01.cpp"} } },
                Module{ "MyLib02", ModuleType::Library, "MyLib02", {}, { {"MyLib02.hpp"}, {"MyLib02.cpp"} } }
            }
        };

        checkIntegrity(package);

        return package;
    }

    Package FileSystemPackageFactory::createPackage(const std::string& sourceDir) {
        assert(boost::filesystem::is_directory(sourceDir));

        Package package;

        const boost::filesystem::path sourcePath{ sourceDir };

        package.name = sourcePath.filename().string();
        package.path = sourcePath;

        Module module;
        module.name = sourcePath.filename().string();
        module.type = ModuleType::Executable;
        module.lang = "c++/17";
        module.path = sourcePath;

        boost::filesystem::recursive_directory_iterator it{ sourcePath };

        while (it != boost::filesystem::end(it)) {
            const boost::filesystem::path sourceFile{ it->path() };

            if (! boost::filesystem::is_regular_file(sourceFile)) {
                it++;
                continue;
            }

            const std::string ext = sourceFile.extension().string();

            if (! isCppExt(ext)) {
                it++;
                continue;
            }

            module.sourceFiles.push_back(SourceFile{ sourceFile.string() });
            it++;
        }

        package.modules = { module };

        return package;
    }
}
