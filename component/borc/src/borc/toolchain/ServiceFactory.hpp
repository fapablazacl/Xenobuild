
#ifndef __BORC_CORE_SERVICEFACTORY_HPP__
#define __BORC_CORE_SERVICEFACTORY_HPP__

#include <vector>
#include <memory>

namespace bok {
    class Compiler;
    class Linker;
    class SourceChecker;
    class ModuleChecker;

    class ServiceFactory {
    public:
        virtual ~ServiceFactory();

        virtual const std::vector<std::pair<SourceChecker*, const Compiler*>> getCompilers() const = 0;

        virtual const std::vector<std::pair<ModuleChecker*, const Linker*>> getLinkers() const = 0;
    };
} 

#endif
