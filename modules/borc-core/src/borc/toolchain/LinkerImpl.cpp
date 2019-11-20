
#include "LinkerImpl.hpp"

namespace borc {
    LinkerImpl::LinkerImpl(const boost::filesystem::path &linkerPath, const std::string &command, const LinkerImplSwitches &switches, const std::vector<LinkerImplBuildRule> &buildRules) {
		this->linkerPath = linkerPath;
		this->command = command;
		this->switches = switches;
		this->buildRules = buildRules;
    }


    LinkerImpl::~LinkerImpl() {}


    LinkOutput LinkerImpl::link(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles) const {
        return {};
    }
}

