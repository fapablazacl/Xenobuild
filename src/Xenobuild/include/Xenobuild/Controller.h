
#pragma once 

#include <Xenobuild/Common.h>

namespace Xenobuild {
    class Controller {
    public:
        virtual ~Controller() {}

        virtual void perform() = 0;
    };
}
