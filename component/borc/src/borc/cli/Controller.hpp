
#ifndef __BORC_CLI_CONTROLLER_HPP__
#define __BORC_CLI_CONTROLLER_HPP__

namespace borc {
    class Controller {
    public:
        virtual ~Controller();

        virtual void perform(int argc, char **argv) = 0;
    };
}

#endif
