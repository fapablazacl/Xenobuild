
#pragma once 

#ifndef __borc_borc_hpp__
#define __borc_borc_hpp__

#include <memory>

namespace borc {

    class Borc {
    public:
        Borc();

        ~Borc();

        int run(int argc, char **argv);

    private:
        struct Private;
        std::unique_ptr<Private> m_impl;
    };
}

#endif
