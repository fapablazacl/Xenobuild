
#pragma once

#ifndef __myapp_mylib01_hpp__
#define __myapp_mylib01_hpp__

namespace myapp {
    class Adder {
    public:
        Adder();
        ~Adder();

        float add(const float a, const float b);
    };
}

#endif
