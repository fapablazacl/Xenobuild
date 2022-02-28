
#pragma once 

#include <memory>
#include <string>
#include <vector>
#include "Controller.h"


namespace Xenobuild {
    struct Context;

    class ControllerFactory {
    public:
        virtual ~ControllerFactory() {}

        virtual std::unique_ptr<Controller> createController(const std::vector<std::string> &args) = 0;
    };

    template<typename ConcreteController>
    class ConcreteControllerFactory : public ControllerFactory {
    public:
        ConcreteControllerFactory(Context& context) : context(context) {}

        std::unique_ptr<Controller> createController(const std::vector<std::string> &args) override {
            const auto params = ConcreteController::Params::parse(args);

            return std::make_unique<ConcreteController>(context, params);
        }

        Context& context;
    };
}
