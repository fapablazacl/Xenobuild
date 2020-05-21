
#ifndef __BOK_TOOLCHAIN_LINKER_HPP__
#define __BOK_TOOLCHAIN_LINKER_HPP__

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Command;
    class Package;
    class Component;

    struct LinkOutput {
        boost::filesystem::path outputModuleRelativePath;
        Command *command = nullptr;
    };

    class Linker {
    public:
        virtual ~Linker();

        virtual bool isModuleLinkable(const Component *component) const {
            return false;
        }

        virtual LinkOutput link(const boost::filesystem::path &outputPath, const Package *package, const Component *component, const std::vector<boost::filesystem::path> &objectFiles) const = 0;
    };
}

#endif
