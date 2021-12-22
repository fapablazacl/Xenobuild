
#pragma once 

#include <memory>
#include "Controller.h"

namespace Xenobuild {
    class Controller;
    class ControllerFactory {
    public:
        virtual ~ControllerFactory() {}

        virtual std::unique_ptr<Controller> createController(const int argc, const char **argv) = 0;
    };

    template<typename ConcreteController>
    class ConcreteControllerFactory {
    public:
        std::unique_ptr<Controller> createController(const int argc, const char **argv) override {
            return std::make_unique<ConcreteController>(argc, argv);
        }
    };
}
