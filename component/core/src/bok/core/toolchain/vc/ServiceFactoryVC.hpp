
#ifndef __BOK_CORE_SERVICEFACTORYVC_HPP__
#define __BOK_CORE_SERVICEFACTORYVC_HPP__

#include <bok/toolchain/ServiceFactoryStub.hpp>

namespace bok {
    class ServiceFactoryVC : public ServiceFactoryStub {
    public:
        ServiceFactoryVC(const std::string &installationPath, const std::string &windowsKitPath);

        virtual ~ServiceFactoryVC();

    private:
        std::unique_ptr<Compiler> createCompiler(const std::string &compilerCommand, const std::string &installationPath, const std::string &windowsKitPath);

        std::unique_ptr<Linker> createLinker(const std::string &linkerCommand, const std::string &installationPath, const std::string &windowsKitPath);
    };
} 

#endif
