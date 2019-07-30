
#include <borc/toolchain/RunService.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <boost/algorithm/string/join.hpp>
#include <borc/model/Artifact.hpp>

namespace borc {
	RunService::RunService(const Compiler *compiler, const Linker *linker) {
		this->compiler = compiler;
		this->linker = linker;
	}

	void RunService::runModule(const Artifact *artifact) {
		// 
		auto deps = artifact->getDependencies();

		std::vector<std::string> paths;
		std::transform(deps.begin(), deps.end(), std::back_inserter(paths), [](const Artifact *dep) {
			// return dep->getOutputPath().string();
			return dep->getPath().string();
		});

		// TODO: Parametrize path separator
		// const std::string pathEnv = join(paths, ":");
		const std::string pathEnv = boost::algorithm::join(paths, ":");
		// const std::string moduleFilePath = artifact->getOutputFilePath().string();
		const std::string moduleFilePath = artifact->getPath().string();

		std::cout << "ENV = " << pathEnv << std::endl;
		std::cout << "EXEC " << moduleFilePath << std::endl;
	}
}
