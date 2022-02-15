
#pragma once 

#include <memory>

#include "Controller.h"


namespace Xenobuild {
    struct Context;

    class ControllerFactory {
    public:
        virtual ~ControllerFactory() {}

        virtual std::unique_ptr<Controller> createController(int argc, char **argv) = 0;
    };

    template<typename ConcreteController>
    class ConcreteControllerFactory : public ControllerFactory {
    public:
        ConcreteControllerFactory(Context& context) : context(context) {}

        std::unique_ptr<Controller> createController(int argc, char **argv) override {
            const auto params = ConcreteController::Params::parse(argc, argv);

            return std::make_unique<ConcreteController>(context, params);
        }

        Context& context;
    };
}
