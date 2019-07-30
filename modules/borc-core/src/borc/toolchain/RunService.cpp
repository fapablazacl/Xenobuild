
#include "RunService.hpp"

#include "Module.hpp"
#include "Common.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace borc {
	RunService::RunService(const Compiler *compiler, const Linker *linker) {
		this->compiler = compiler;
		this->linker = linker;
	}

	void RunService::runModule(const Module *module) {
		// 
		auto deps = module->getDependencies();

		std::vector<std::string> paths;
		std::transform(deps.begin(), deps.end(), std::back_inserter(paths), [](const Module *dep) {
			return dep->getOutputPath().string();
		});

		// TODO: Parametrize path separator
		const std::string pathEnv = join(paths, ":");
		const std::string moduleFilePath = module->getOutputFilePath().string();

		std::cout << "ENV = " << pathEnv << std::endl;
		std::cout << "EXEC " << moduleFilePath << std::endl;
	}
}
