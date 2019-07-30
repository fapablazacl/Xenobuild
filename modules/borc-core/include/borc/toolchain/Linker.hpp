
#ifndef __BORC_MODEL_LINKER_HPP__
#define __BORC_MODEL_LINKER_HPP__

#include <string>
#include <vector>


namespace borc {
	class CommandFactory;
	class Package;
	class Artifact;

	struct LinkerSwitches {
		std::string buildSharedLibrary;
		std::string moduleOutput;
		std::string importLibrary;
		std::string importLibraryPath;

		LinkerSwitches() {}
	};

	struct LinkerConfiguration {
		std::vector<std::string> importLibraries;
		std::vector<std::string> importLibraryPaths;

		LinkerConfiguration() {}
	};

	class Linker {
	public:
		explicit Linker(CommandFactory *commandFactory, const std::string &commandPath, const LinkerSwitches &switches, const LinkerConfiguration &configuration);

		std::string link(const Package *package, const Artifact *artifact, const std::vector<std::string> &objectFiles) const;

	private:
		std::vector<std::string> collectLibraries(const Package *package, const Artifact *artifact) const;
		std::vector<std::string> collectLibraryPaths(const Package *package, const Artifact *artifact) const;
		
		std::vector<std::string> computeLibrariesOptions(const std::vector<std::string> &libraries) const;
		std::vector<std::string> computeLibraryPathsOptions(const std::vector<std::string> &libraryPaths) const;

	private:
		CommandFactory *commandFactory = nullptr;
		std::string commandPath;
		LinkerSwitches switches;
		LinkerConfiguration configuration;
	};
}

#endif
