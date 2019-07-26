
#ifndef __BORC_CLI_CLICONTROLLERIMPL_HPP__
#define __BORC_CLI_CLICONTROLLERIMPL_HPP__

#include "CliController.hpp"

namespace borc {
    class CliControllerImpl : public CliController {
    public:
        virtual ~CliControllerImpl();

        virtual void dispatch(const std::string &subcommand, const std::vector<std::string> &args) override;

        virtual std::set<std::string> enumerateSubcommands() const override;

    private:
        void configure();
    };
}

#endif
