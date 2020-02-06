
#ifndef __BORC_CORE_CORE_HPP__
#define __BORC_CORE_CORE_HPP__

#include <memory>
#include <boost/filesystem/path.hpp>

namespace borc {
    class ToolchainFactory;
    class ToolchainService;

    class Core {
    public:
        explicit Core(const boost::filesystem::path &toolchainDefinitionPath);

        ~Core();

        ToolchainFactory* getToolchainFactory();

    private:

    
    private:
        boost::filesystem::path toolchainDefinitionPath;
        ToolchainService *toolchainService = nullptr;

        std::unique_ptr<ToolchainFactory> toolchainFactory;
    };
}

#endif