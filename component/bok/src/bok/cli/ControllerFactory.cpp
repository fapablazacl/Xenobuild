
#include "ControllerFactory.hpp"

#include <stdexcept>
#include <bok/cli/InitController.hpp>
#include <bok/cli/BuildController.hpp>
#include <bok/cli/BuildController2.hpp>
#include <bok/cli/ConfigureController.hpp>

namespace bok {
    template<typename ControllerImpl>
    class ControllerMakerImpl : public ControllerMaker {
    public:
        virtual std::unique_ptr<Controller> make() override {
            return std::make_unique<ControllerImpl>();
        }
    };

    ControllerFactory::ControllerFactory() {
        // makers["build"] = std::make_unique<ControllerMakerImpl<BuildController>>();
        makers["build"] = std::make_unique<ControllerMakerImpl<BuildController2>>();
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
