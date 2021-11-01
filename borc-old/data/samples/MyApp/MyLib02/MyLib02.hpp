
#pragma once

#ifndef __myapp_mylib02_hpp__
#define __myapp_mylib02_hpp__

namespace myapp {
    class Multiplier {
    public:
        Multiplier();
        ~Multiplier();

        float multiply(const float a, const float b);
    };
}

#endif
