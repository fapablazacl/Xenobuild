
#include "ControllerFactory.hpp"

#include <stdexcept>
#include <borc/cli/init/InitController.hpp>
#include <borc/cli/build/BuildController.hpp>
#include <borc/cli/configure/ConfigureController.hpp>

namespace borc {
    template<typename ControllerImpl>
    class ControllerMakerImpl : public ControllerMaker {
    public:
        virtual std::unique_ptr<Controller> make() override {
            return std::make_unique<ControllerImpl>();
        }
    };

    ControllerFactory::ControllerFactory() {
        makers["build"] = std::make_unique<ControllerMakerImpl<BuildController>>();
        makers["configure"] = std::make_unique<ControllerMakerImpl<ConfigureController>>();
        makers["init"] = std::make_unique<ControllerMakerImpl<InitController>>();
    }

    ControllerFactory::~ControllerFactory() {}

    std::unique_ptr<Controller> ControllerFactory::create(const std::string &name) {
        if (auto makerIt = makers.find(name); makerIt != makers.end()) {
            return makerIt->second->make();
        }
        
        throw std::runtime_error("Unknown subcommand '" + name + "'");
    }
}
