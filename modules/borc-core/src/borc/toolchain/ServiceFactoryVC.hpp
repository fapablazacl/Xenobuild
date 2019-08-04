
#ifndef __BORC_CORE_SERVICEFACTORYVC_HPP__
#define __BORC_CORE_SERVICEFACTORYVC_HPP__

#include <borc/model/CommandFactory.hpp>
#include <borc/toolchain/ServiceFactory.hpp>

namespace borc {
	class ServiceFactoryVC : public ServiceFactory {
	public:
		ServiceFactoryVC(const std::string &installationPath, const std::string &windowsKitPath);

		virtual ~ServiceFactoryVC();

		virtual const Compiler* getCompiler() const override;

		virtual const Linker* getLinker() const override;

	private:
		std::unique_ptr<Compiler> createCompiler(const std::string &compilerCommand, const std::string &installationPath, const std::string &windowsKitPath);

		std::unique_ptr<Linker> createLinker(const std::string &linkerCommand, const std::string &installationPath, const std::string &windowsKitPath);

	private:
		std::unique_ptr<Compiler> compiler;
		std::unique_ptr<Linker> linker;

		CommandFactory commandFactory;
	};
} 

#endif
