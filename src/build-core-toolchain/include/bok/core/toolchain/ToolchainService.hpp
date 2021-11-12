
#ifndef __BOK_SERVICES_TOOLCHAINSERVICE_HPP__
#define __BOK_SERVICES_TOOLCHAINSERVICE_HPP__

#include <memory>
#include <boost/optional/optional.hpp>
#include <boost/filesystem/path.hpp>

#include "Toolchain.hpp"

namespace bok {
    class Toolchain;

    class ToolchainService {
    public:
        virtual ~ToolchainService();

        virtual std::unique_ptr<Toolchain> createToolchain(const boost::filesystem::path &toolchainFolder, boost::optional<boost::filesystem::path> installationPath) const = 0;
    }; 
}

#endif
