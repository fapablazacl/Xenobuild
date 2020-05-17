
#ifndef __BORC_CLI_CONTROLLERFACTORY_HPP__
#define __BORC_CLI_CONTROLLERFACTORY_HPP__

#include <string>
#include <map>
#include <memory>

namespace bok {
    class Controller;

    class ControllerMaker {
    public:
        virtual ~ControllerMaker() = default;

        virtual std::unique_ptr<Controller> make() = 0;
    };

    class ControllerFactory {
    public:
        ControllerFactory();

        ~ControllerFactory();

        std::unique_ptr<Controller> create(const std::string &name);

    private:
        std::map<std::string, std::unique_ptr<ControllerMaker>> makers;
    };
}

#endif
