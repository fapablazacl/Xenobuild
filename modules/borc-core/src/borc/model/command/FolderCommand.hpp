
#ifndef __BORC_MODEL_COMMAND_FOLDERCOMMAND_HPP__
#define __BORC_MODEL_COMMAND_FOLDERCOMMAND_HPP__

#include <borc/model/Command.hpp>
#include <borc/model/CommandFactory.hpp>
#include <boost/filesystem/path.hpp>

namespace borc {
	class FolderCommand : public Command {
    public:
		explicit FolderCommand(const boost::filesystem::path &path, const PathCommand pathCommand);

		virtual void execute() override;

	private:
		boost::filesystem::path path;
		PathCommand pathCommand;
	};
}

#endif
