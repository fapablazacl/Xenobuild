
#pragma once 

#include <string>
#include <vector>
#include "Package.hpp"
#include <borc/toolchain/Toolchain.hpp>
#include <borc/common/Types.hpp>

namespace borc {
    struct Context {
        Package *mainPackage = nullptr;
        std::vector<std::unique_ptr<Package>> packages;
        std::vector<std::unique_ptr<Toolchain>> toolchains;

        static Context create(const path &rootPath);
    };
}
