
#ifndef __BORC_CORE_SERVICEFACTORYVC_HPP__
#define __BORC_CORE_SERVICEFACTORYVC_HPP__

#include <borc/core/CommandFactory.hpp>
#include <borc/core/ServiceFactory.hpp>

namespace borc {
	class ServiceFactoryVC : public ServiceFactory {
	public:
		ServiceFactoryVC(const std::string &installationPath, const std::string &windowsKitPath);

		virtual ~ServiceFactoryVC();

		virtual BuildService createBuildService() override;

		virtual RunService createRunService() override;

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
