
#ifndef __BORC_CLI_CONFIGURECONTROLLER_HPP__
#define __BORC_CLI_CONFIGURECONTROLLER_HPP__

#include "Controller.hpp"

#include <boost/filesystem/path.hpp>

namespace borc {
    class ConfigureController : public Controller {
    public:
        virtual ~ConfigureController();

        virtual void perform(int argc, char **argv) override;

    private:
        bool checkValidBorcFile(const boost::filesystem::path &filePath) const;
    };
}

#endif
