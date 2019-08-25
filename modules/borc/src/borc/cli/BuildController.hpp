
#ifndef __BORC_CLI_BUILDCONTROLLER_HPP__
#define __BORC_CLI_BUILDCONTROLLER_HPP__

#include "Controller.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/optional/optional.hpp>

namespace borc {
    struct PackageEntity;
    struct ModuleEntity;
    struct BuildControllerOptions;

    class Package;
    class FileService;

    class BuildController : public Controller {
    public:
        virtual ~BuildController();

        virtual void perform(int argc, char **argv) override;
    };
}

#endif
