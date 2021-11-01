
#include "BorcFile.hpp"

#include <fstream>
#include <iostream>

#include "pod/Buildable.hpp"
#include "pod/Project.hpp"

#include "BorcFile.yy.hpp"

namespace borc {
    struct BorcFile::Private {
        Project project;
    };

    BorcFile::BorcFile() 
        : m_impl(new BorcFile::Private()) {}

    BorcFile::~BorcFile() {}

    const Project& BorcFile::getProjectRef() const {
        return m_impl->project;
    }

    static std::string loadFile(const std::string &file) {
        std::string content;

        std::fstream fs;
        std::string line;

        fs.open(file.c_str(), std::ios_base::in);
        if (!fs.is_open()) {
            throw std::runtime_error("borc::BorcFile::get: File " + file + " wasn't found.");
        }

        while(!fs.eof()) {
            std::getline(fs, line);

            content += line;
        }
    }

    std::unique_ptr<BorcFile> BorcFile::get(const std::string &file) {
        std::string content = loadFile(file);

        auto borcFile = std::make_unique<BorcFile>();

        //yy_scan_buffer(content.c_str());
        //yyin = content.c_str();
        yyparse();

        return borcFile;
    }
}
