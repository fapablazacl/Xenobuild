
#pragma once 

#ifndef __borc_buildable_hpp__
#define __borc_buildable_hpp__

#include <memory>
#include <string>
#include <vector>

namespace borc {

    /**
     * @brief A buildable entity
     */
    struct Buildable {
        //! The name of the program
        std::string name;

        //! The current relative path. (No .. notation allowed!)
        std::string path;

        //! Array of relative private include files 
        std::vector<std::string> includes;

        //! Array of relative public include files 
        std::vector<std::string> public_includes;

        //! Array of source code files of the current buildable entity
        std::vector<std::string> sources;

        //! Array of needed dependencies for build/run this entity
        std::vector<std::string> imports;

        Buildable();
    };
}

#endif
