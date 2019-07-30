
#include "CommandFactory.hpp"

#include "SystemCommand.hpp"
// #include "ProcessCommand.hpp"

namespace borc {
	CommandFactory::~CommandFactory() {}

	Command* CommandFactory::createCommand(const std::string &base, const std::vector<std::string> &options) {
		auto command = new SystemCommand(base, options);

		_cachedCommands.emplace_back(command);

		return command;
	}
}
