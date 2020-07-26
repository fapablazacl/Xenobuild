
#ifndef __BOK_SERVICES_BUILDSERVICEIMPL_HPP__
#define __BOK_SERVICES_BUILDSERVICEIMPL_HPP__

#include "TaskGraphGenerator.hpp"

#include <vector>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Toolchain;
    class Logger;
    class Module;
    class BuildCache;
    class Command;

    struct CompileOptions;

    class TaskGraphGenerator_Build : public TaskGraphGenerator {
    public:
        TaskGraph generate(Toolchain* toolchain, Module *module) const override;
    };
}

#endif
