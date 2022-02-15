
#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include "ToolchainType.h"
#include "Triplet.h"


namespace Xenobuild {
    struct Triplet;
    struct CommandX;
    struct CommandBatch;

    class ToolchainInstallPathEnumerator {
    public:
        /**
         * @brief Enumerate the installation paths for the given toolchain type, ordered from the latest version.
         */
        std::vector<std::string> enumerate(const ToolchainType type) const;
    };

    /**
     * @brief Compiler toolchain abstraction.
     */
    class Toolchain {
    public:
        explicit Toolchain(const Triplet &triplet, const std::string &installPath);

        /**
         * @brief Creates a new Command, which setups all the required environment variables
         * for invoking the toolchain compilers, linkers, and related tools in a later time.
         */
        CommandX createEnvCommand() const;

    private:
        Triplet triplet;
        std::string installPath;
    };
}
