
#ifndef __BORC_CORE_SERVICEFACTORYSTUB_HPP__
#define __BORC_CORE_SERVICEFACTORYSTUB_HPP__

#include <borc/toolchain/ServiceFactory.hpp>

#include <borc/model/CommandFactory.hpp>

namespace borc {
	class ServiceFactoryStub : public ServiceFactory {
	public:
		virtual ~ServiceFactoryStub();

		virtual const std::vector<std::pair<SourceChecker*, const Compiler*>> getCompilers() const override;

		virtual const std::vector<std::pair<ArtifactChecker*, const Linker*>> getLinkers() const override;

	protected:
		CommandFactory commandFactory;
        std::vector<std::pair<std::unique_ptr<SourceChecker>, std::unique_ptr<Compiler>>> compilers;
        std::vector<std::pair<std::unique_ptr<ArtifactChecker>, std::unique_ptr<Linker>>> linkers;
	};
}

#endif
