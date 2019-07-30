
#ifndef __BORC_MODEL_RUNSERVICE_HPP__
#define __BORC_MODEL_RUNSERVICE_HPP__



namespace borc {
	class Compiler;
	class Linker;
	class Artifact;

	class RunService {
	public:
		explicit RunService(const Compiler *compiler, const Linker *linker);

		void runModule(const Artifact *artifact);

	private:
		const Compiler *compiler;
		const Linker *linker;
	};
}

#endif
