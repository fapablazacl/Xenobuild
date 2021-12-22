
#pragma once 

#include "Controller.h"

namespace Xenobuild {
    class BuildController : public Controller {
    public:
        BuildController(int argc, char **argv);

        void perform() override;
    };
}
