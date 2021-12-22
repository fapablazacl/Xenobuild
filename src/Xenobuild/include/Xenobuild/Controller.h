
#pragma once 

namespace Xenobuild {
    class Controller {
    public:
        virtual ~Controller() {}

        virtual void perform() = 0;
    };
}
