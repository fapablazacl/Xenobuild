
#ifndef __BORC_MODEL_COMMANDFACTORY_HPP__
#define __BORC_MODEL_COMMANDFACTORY_HPP__

#include <string>
#include <vector>
#include <memory>


namespace borc {
	class Command;
	class CommandFactory {
	public:
		~CommandFactory();

		Command* createCommand(const std::string &base, const std::vector<std::string> &options = {});

	private:
		std::vector<std::unique_ptr<Command>> _cachedCommands;
	};
}

#endif
