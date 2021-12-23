
#pragma once 

#include <memory>

#include "Controller.h"
#include <Xenobuild/core/PackageFactory.h>


namespace Xenobuild {
    class ControllerFactory {
    public:
        virtual ~ControllerFactory() {}

        virtual std::unique_ptr<Controller> createController(int argc, char **argv) = 0;
    };

    template<typename ConcreteController>
    class ConcreteControllerFactory : public ControllerFactory {
    public:
        ConcreteControllerFactory(PackageFactory& packageFactory) 
            : packageFactory(packageFactory) {}

        std::unique_ptr<Controller> createController(int argc, char **argv) override {
            const auto params = ConcreteController::Params::parse(argc, argv);

            return std::make_unique<ConcreteController>(packageFactory, params);
        }

        PackageFactory& packageFactory;
    };
}
