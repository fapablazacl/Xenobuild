
#include "ControllerFactory.hpp"

#include <stdexcept>
#include "ConfigureController.hpp"

namespace borc {
    template<typename ControllerImpl>
    class ControllerMakerImpl : public ControllerMaker {
    public:
        virtual std::unique_ptr<Controller> make() override {
            return std::make_unique<ControllerImpl>();
        }
    };

    ControllerFactory::ControllerFactory() {
        makers["configure"] = std::make_unique<ControllerMakerImpl<ConfigureController>>();
    }

    ControllerFactory::~ControllerFactory() {}

    std::unique_ptr<Controller> ControllerFactory::create(const std::string &name) {
        if (auto makerIt = makers.find(name); makerIt != makers.end()) {
            return makerIt->second->make();
        }
        
        throw std::runtime_error("Unknown subcommand " + name + "");
    }
}
