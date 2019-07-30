
#include <borc/toolchain/BuildService.hpp>

#include <algorithm>
#include <boost/filesystem/path.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/model/Package.hpp>
#include <borc/toolchain/Compiler.hpp>
#include <borc/toolchain/Linker.hpp>

namespace borc {
	BuildService::BuildService(const Compiler *compiler, const Linker *linker) {
		this->compiler = compiler;
		this->linker = linker;
	}

	static CompileOptions computeCompileOptions(const Artifact *artifact) {
		std::vector<std::string> directories;

		for (const Artifact *artifact : artifact->getDependencies()) {
			const std::string directory = artifact->getPath().string();

			directories.push_back(directory);
		}

		return { directories };
	}

	void BuildService::buildProject(const Package *package) {
		int builtModules = 0;

		for (const Artifact *artifact : package->getArtifacts()) {
			const CompileOptions compileOptions = computeCompileOptions(artifact);

			std::cout << "Building module " << artifact->getName() << " ..." << std::endl;

			const auto files = artifact->getSourceFiles();

			std::vector<std::string> objectFiles;
			for (const boost::filesystem::path &file : files) {
				if (this->isFileCompilable(file.string())) {
					objectFiles.push_back(file.string());
				}
			}

			std::transform(objectFiles.begin(), objectFiles.end(), objectFiles.begin(), [&](const std::string &file) {
				return this->compiler->compile(package, artifact, file, compileOptions);
			});

			linker->link(package, artifact, objectFiles);

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
