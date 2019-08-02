
#include <borc/model/command/ProcessCommand.hpp>

#include <iostream>
#include <stdexcept>
#include <sstream>

namespace borc {
	ProcessCommand::ProcessCommand(const std::string &base)
		: _base(base) {}

	ProcessCommand::ProcessCommand(const std::string &base, const std::vector<std::string> &options)
		: _base(base), _options(options) {}

	void ProcessCommand::execute() {
		throw std::runtime_error("ProcessCommand::execute is not implemented.");
	}

	void ProcessCommand::addOption(const std::string &option) {
		_options.push_back(option);
	}
}
