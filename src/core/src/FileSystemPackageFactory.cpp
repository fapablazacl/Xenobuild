
#include <Xenobuild/core/FileSystemPackageFactory.h>
#include <Xenobuild/core/Module.h>
#include <Xenobuild/core/Dependency.h>

#include <cassert>
#include <iostream>
#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/Module.h>


namespace Xenobuild {
    static bool isCppExt(const std::string& ext) {
        return ext == ".cpp" || ext == ".h" || ext == ".c" || ext == ".hpp";
    }


    /*
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
    */


    Package FileSystemPackageFactory::createMockPackage() {
        return {};

        /*
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
        */
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


namespace YAML {
    template<typename T>
    inline bool extractValue(const YAML::Node& node, const std::string& name, T& value) {
        if (! node[name]) {
            return false;
        }

        value = node[name].as<T>();

        return true;
    }


    template<typename T>
    inline bool extractValue(const YAML::Node& node, const std::string& name, T& value, const T defaultValue) {
        if (! node[name]) {
            value = defaultValue;
            return false;
        }

        value = node[name].as<T>();

        return true;
    }
    
    template<> struct convert<boost::filesystem::path> {
        static Node encode(const boost::filesystem::path& rhs) {
            Node node;
            
            node = rhs.string();
            
            return node;
        }

        static bool decode(const Node& node, boost::filesystem::path& rhs) {
            if (! node.IsScalar()) {
                return false;
            }
            
            const std::string tentative = node.as<std::string>();
            
            if (! boost::filesystem::native(tentative)) {
                return false;
            }
            
            rhs = tentative;
            
            return true;
        }
    };
    
    template<> struct convert<Xenobuild::Package> {
        static Node encode(const Xenobuild::Package& rhs) {
            Node node;

            node["name"] = rhs.name;
            node["path"] = rhs.path;
            node["dependencies"] = rhs.dependencies;
            
            return node;
        }

        static bool decode(const Node& node, Xenobuild::Package& rhs) {
            if (node.IsScalar()) {
                return false;
            }

            extractValue(node, "name", rhs.name);
            extractValue(node, "path", rhs.path);
            extractValue(node, "dependencies", rhs.dependencies);
            
            return true;
        }
    };
    
    template<> struct convert<Xenobuild::Dependency> {
        static Node encode(const Xenobuild::Dependency& rhs) {
            Node node;

            node["url"] = rhs.url;
            node["tag"] = rhs.tag;
            node["version"] = rhs.version;
            node["definitions"] = rhs.definitions;
            node["publicdefs"] = rhs.publicdefs;
            
            return node;
        }

        static bool decode(const Node& node, Xenobuild::Dependency& rhs) {
            if (node.IsScalar()) {
                return false;
            }

            extractValue(node, "url", rhs.url);
            extractValue(node, "tag", rhs.tag);
            extractValue(node, "version", rhs.version);
            extractValue(node, "definitions", rhs.definitions);
            extractValue(node, "publicdefs", rhs.publicdefs);
            
            return true;
        }
    };
}


namespace Xenobuild {
    Package FileSystemPackageFactory::createPackage(std::istream &is) {
        YAML::Node rootNode = YAML::Load(is);
        
        Package package = rootNode.as<Package>();
        
        return package;
    }
}
