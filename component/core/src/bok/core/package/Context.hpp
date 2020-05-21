
#pragma once 

#include <string>
#include <vector>
#include "Package.hpp"
#include <bok/core/toolchain/Toolchain.hpp>
#include <bok/core/Types.hpp>

namespace bok {
    struct Context {
        Package *mainPackage = nullptr;
        std::vector<std::unique_ptr<Package>> packages;
        std::vector<std::unique_ptr<Toolchain>> toolchains;

        static Context create(const path &rootPath);
    };
}
