
#include "SystemCommand.hpp"

#include <iostream>
#include <stdexcept>
#include <boost/algorithm/string/join.hpp>

namespace borc {
	SystemCommand::SystemCommand(const std::string &base)
		: _base(base) {}

	SystemCommand::SystemCommand(const std::string &base, const std::vector<std::string> &options)
		: _base(base), _options(options) {}

	void SystemCommand::execute() {
		// const std::string systemCommand = _base + " " + join(_options, " ");
		const std::string systemCommand = _base + " " + boost::algorithm::join(_options, " ");

		std::cout << systemCommand << std::endl;

		const int exitCode = std::system(systemCommand.c_str());

		if (exitCode != 0) {
			std::string msg;

			msg += "The command \n";
			msg += "'" + systemCommand + "'\n";
			msg += "returned an erroneous exit code: " + std::to_string(exitCode);

			throw std::runtime_error(msg);
		}
	}
}
