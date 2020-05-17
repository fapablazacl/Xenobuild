
#ifndef __BORC_CORE_SERVICEFACTORYGCC_HPP__
#define __BORC_CORE_SERVICEFACTORYGCC_HPP__

#include <borc/toolchain/ServiceFactoryStub.hpp>

namespace bok {
    class ServiceFactoryGCC : public ServiceFactoryStub {
    public:
        explicit ServiceFactoryGCC(const std::string &commandBase);

        virtual ~ServiceFactoryGCC();

    private:
        std::unique_ptr<Compiler> createCompiler();

        std::unique_ptr<Linker> createLinker();

        std::unique_ptr<Linker> createStaticLinker();

    private:
        std::string commandBase;
    };
} 

#endif
