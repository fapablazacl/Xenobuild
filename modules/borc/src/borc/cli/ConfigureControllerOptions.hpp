
#ifndef __BORC_CLI_CONFIGURECONTROLLEROPTIONS_HPP__
#define __BORC_CLI_CONFIGURECONTROLLEROPTIONS_HPP__

namespace borc {
    struct ConfigureControllerOptions {
        static ConfigureControllerOptions parse(int argc, char **argv);
    };
}

#endif
