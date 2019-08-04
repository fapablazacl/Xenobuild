
#ifndef __BORC_TOOLCHAIN_LINKERIMPL_HPP__
#define __BORC_TOOLCHAIN_LINKERIMPL_HPP__

#include <borc/toolchain/Linker.hpp>

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace borc {
	class Command;
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

	class LinkerImpl : public Linker {
	public:
		explicit LinkerImpl(CommandFactory *commandFactory, const std::string &commandPath, const LinkerSwitches &switches, const LinkerConfiguration &configuration);

        virtual ~LinkerImpl();

		virtual LinkOutput link(const Package *package, const Artifact *artifact, const std::vector<boost::filesystem::path> &objectFiles) const override;

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
