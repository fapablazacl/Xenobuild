
#pragma once 

#ifndef __borc_project_hpp__
#define __borc_project_hpp__

#include "Buildable.hpp"

namespace borc {
    struct Project {
        std::vector<Buildable> buildables;

        Project();
    };
}

#endif 
