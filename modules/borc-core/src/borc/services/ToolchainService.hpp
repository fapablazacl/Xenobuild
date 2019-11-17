
#ifndef __BORC_SERVICES_TOOLCHAINSERVICE_HPP__
#define __BORC_SERVICES_TOOLCHAINSERVICE_HPP__

#include <memory>
#include <boost/filesystem/path.hpp>

namespace borc {
    class Toolchain;

    class ToolchainService {
    public:
        virtual ~ToolchainService();

        virtual std::unique_ptr<Toolchain> createToolchain(const boost::filesystem::path &toolchainFolder) const = 0;
    }; 
}

#endif
