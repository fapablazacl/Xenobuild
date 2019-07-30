
#include "BuildService.hpp"

#include <algorithm>
#include <boost/filesystem.hpp>

#include "Project.hpp"
#include "Module.hpp"
#include "Compiler.hpp"
#include "Linker.hpp"

namespace borc {
	BuildService::BuildService(const Compiler *compiler, const Linker *linker) {
		this->compiler = compiler;
		this->linker = linker;
	}

	static CompileOptions computeCompileOptions(const Module *module) {
		std::vector<std::string> directories;

		for (Module *module : module->getDependencies()) {
			const std::string directory = module->getPath().string();

			directories.push_back(directory);
		}

		return { directories };
	}

	void BuildService::buildProject(const Project *project) {
		auto modules = project->getModules();

		int builtModules = 0;

		for (const Module *module : modules) {
			const CompileOptions compileOptions = computeCompileOptions(module);

			std::cout << "Building module " << module->getName() << " ..." << std::endl;

			const auto files = module->getFiles();

			std::vector<std::string> objectFiles;
			std::copy_if(files.begin(), files.end(), std::back_inserter(objectFiles), [&](const auto &file) {
				return this->isFileCompilable(file);
			});

			std::transform(objectFiles.begin(), objectFiles.end(), objectFiles.begin(), [&](const auto &file) {
				return this->compiler->compile(project, module, file, compileOptions);
			});

			linker->link(project, module, objectFiles);

			builtModules++;
		}

		std::cout << "Built " << builtModules << " module(s)." << std::endl;
	}

	bool BuildService::isFileCompilable(const std::string &file) const {
		const std::string ext = "*" + boost::filesystem::path(file).extension().string();

		auto it = std::find(compilerWildcards.begin(), compilerWildcards.end(), ext);

		return it != compilerWildcards.end();
	}
}
