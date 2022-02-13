
#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>


namespace Xenobuild {
    struct Toolchain;
    struct CommandBatch;

    class ToolchainCommandExecutor {
    public:
        ToolchainCommandExecutor();

        CommandBatch createCommandBatch();
    };
}
