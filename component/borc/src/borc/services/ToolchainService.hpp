
#ifndef __BORC_SERVICES_TOOLCHAINSERVICE_HPP__
#define __BORC_SERVICES_TOOLCHAINSERVICE_HPP__

#include <memory>
#include <boost/optional/optional_fwd.hpp>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Toolchain;

    class ToolchainService {
    public:
        virtual ~ToolchainService();

        virtual std::unique_ptr<Toolchain> createToolchain(const boost::filesystem::path &toolchainFolder, boost::optional<boost::filesystem::path> installationPath) const = 0;
    }; 
}

#endif
