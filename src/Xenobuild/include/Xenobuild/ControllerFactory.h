
#pragma once 

#include <memory>
#include "Controller.h"

namespace Xenobuild {
    class ControllerFactory {
    public:
        virtual ~ControllerFactory() {}

        virtual std::unique_ptr<Controller> createController(int argc, char **argv) = 0;
    };

    template<typename ConcreteController>
    class ConcreteControllerFactory : public ControllerFactory {
    public:
        std::unique_ptr<Controller> createController(int argc, char **argv) override {
            return std::make_unique<ConcreteController>(argc, argv);
        }
    };
}
