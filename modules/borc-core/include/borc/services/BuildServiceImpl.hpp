
#ifndef __BORC_SERVICES_BUILDSERVICEIMPL_HPP__
#define __BORC_SERVICES_BUILDSERVICEIMPL_HPP__

#include "BuildService.hpp"

#include <boost/filesystem/path.hpp>

namespace borc {
    class Toolchain;

    class BuildServiceImpl : public BuildService {
    public:
        BuildServiceImpl(const boost::filesystem::path &outputFolder, Toolchain *toolchain);

        virtual ~BuildServiceImpl();

        virtual void build(Package *package) override;

    private:
        boost::filesystem::path outputFolder;
        Toolchain *toolchain = nullptr;
    };
}

#endif
