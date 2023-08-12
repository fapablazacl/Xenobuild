
#pragma once

#include <memory>
#include <bok/cli/Controller.hpp>

namespace bok {
    class Logger;
    class FileService;
    class PackageFactory;
    class PackageRegistryFactory;
    class PackageRegistry;

    class BuildController : public Controller {
    public:
        explicit BuildController();

        virtual ~BuildController();

        virtual void perform(int argc, char **argv) override;

    private:
        Logger *logger = nullptr;
        FileService *fileService = nullptr;
        PackageFactory *packageFactory = nullptr;
        PackageRegistryFactory *packageRegistryFactory = nullptr;

        std::unique_ptr<PackageRegistry> packageRegistry;
    };
}
