
#ifndef __BORC_CORE_SERVICEFACTORYGCC_HPP__
#define __BORC_CORE_SERVICEFACTORYGCC_HPP__

#include <borc/model/CommandFactory.hpp>
#include <borc/toolchain/ServiceFactory.hpp>

namespace borc {
	class ServiceFactoryGCC : public ServiceFactory {
	public:
		explicit ServiceFactoryGCC(const std::string &commandBase);

	private:
		std::unique_ptr<Compiler> createCompiler();

		std::unique_ptr<Linker> createLinker();

		virtual const Compiler* getCompiler() const override;

		virtual const Linker* getLinker() const override;

	private:
		std::string commandBase;
		std::unique_ptr<Compiler> compiler;
		std::unique_ptr<Linker> linker;

		CommandFactory commandFactory;
	};
} 

#endif
