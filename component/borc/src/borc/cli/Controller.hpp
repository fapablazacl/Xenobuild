
#ifndef __BOK_CLI_CONTROLLER_HPP__
#define __BOK_CLI_CONTROLLER_HPP__

namespace bok {
    class Controller {
    public:
        virtual ~Controller();

        virtual void perform(int argc, char **argv) = 0;
    };
}

#endif
