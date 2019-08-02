
#include <borc/toolchain/CommandFactory.hpp>

#include <iostream>
#include <boost/filesystem.hpp>
#include <borc/toolchain/SystemCommand.hpp>

namespace borc {
	class PathCommandImpl : public Command {
	public:
		explicit PathCommandImpl(const boost::filesystem::path &path, const PathCommand pathCommand) {
			this->path = path;
			this->pathCommand = pathCommand;
		}


		virtual void execute() override {
			switch (pathCommand) {
				case PathCommand::Create: 
					boost::filesystem::create_directories(path); 
					break;
			}
		}

		virtual void addOption(const std::string &option) override {
			std::cout << "PathCommandImpl::addOption: Meaningless operation. Command base class needs refactor!" << std::endl;
		}

	private:
		boost::filesystem::path path;
		PathCommand pathCommand;
	};


	CommandFactory::~CommandFactory() {}


	Command* CommandFactory::createCommand(const std::string &base, const std::vector<std::string> &options) {
		auto command = new SystemCommand(base, options);

		_cachedCommands.emplace_back(command);

		return command;
	}


	Command* CommandFactory::createPathCommand(const boost::filesystem::path &path, const PathCommand pathCommand) {
		auto command = new PathCommandImpl(path, pathCommand);

		_cachedCommands.emplace_back(command);

		return command;
	}
}
