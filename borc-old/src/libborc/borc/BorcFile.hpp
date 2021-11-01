
#pragma once 

#ifndef __borc_borcfile_hpp__
#define __borc_borcfile_hpp__

#include <memory>
#include <string>

namespace borc {

    struct Project;

    /**
     * @brief Hold the content of a Borc File 
     */
    class BorcFile {
    public:
        BorcFile();

        ~BorcFile();

        const Project& getProjectRef() const;

    public:
        static std::unique_ptr<BorcFile> get(const std::string &file); 

    private:
        struct Private;
        std::unique_ptr<Private> m_impl;
    };
}

#endif
