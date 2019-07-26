
#ifndef __BORC_CLI_CLICONTROLLER_HPP__
#define __BORC_CLI_CLICONTROLLER_HPP__

#include <string>
#include <set>
#include <vector>
#include <optional>

namespace borc {
    struct Configuration {
        //! The toolchain. If not set, we automatically select one, dependening on the current platform.
        std::optional<std::string> toolchain;
    };

    class CliController {
    public:
        virtual ~CliController();

        virtual void dispatch(const std::string &subcommand, const std::vector<std::string> &args) = 0;

        virtual std::set<std::string> enumerateSubcommands() const = 0;

        // put this on a new controller interface.
        // virtual void configure(const Configuration &config) = 0;
    };
}

#endif
