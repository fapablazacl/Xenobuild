
#ifndef __BOK_CORE_SERVICEFACTORYSTUB_HPP__
#define __BOK_CORE_SERVICEFACTORYSTUB_HPP__

#include <bok/core/toolchain/ServiceFactory.hpp>
#include <bok/core/common/CommandFactory.hpp>

namespace bok {
    class ServiceFactoryStub : public ServiceFactory {
    public:
        virtual ~ServiceFactoryStub();

        virtual const std::vector<std::pair<SourceChecker*, const Compiler*>> getCompilers() const override;

        virtual const std::vector<std::pair<ModuleChecker*, const Linker*>> getLinkers() const override;

    protected:
        CommandFactory commandFactory;
        std::vector<std::pair<std::unique_ptr<SourceChecker>, std::unique_ptr<Compiler>>> compilers;
        std::vector<std::pair<std::unique_ptr<ModuleChecker>, std::unique_ptr<Linker>>> linkers;
    };
}

#endif
