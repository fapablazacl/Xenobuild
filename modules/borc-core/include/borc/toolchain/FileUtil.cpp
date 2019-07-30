
#include "FileUtil.hpp"

#include <fstream>

namespace borc {
    std::string FileUtil::load(const std::string &filename) {
        typedef std::istreambuf_iterator<char> fstream_iterator;

        std::fstream fs;

        fs.open(filename.c_str(), std::ios_base::in);
        if (!fs.is_open()) {
            throw std::runtime_error("Couldn't open the file '" + filename + "'.");
        }

        fs.seekg(0);

        std::string content;
        content.assign(fstream_iterator(fs), fstream_iterator());

        return content;
    }

    void FileUtil::save(const std::string &filename, const std::string &content) {
        std::fstream fs;

        fs.open(filename.c_str(), std::ios_base::out);
        if (!fs.is_open()) {
            throw std::runtime_error("The file \"" + filename + "\" could't be opened");
        }

        if (content.size() > 0) {
            fs.write(content.c_str(), content.size());
        } else {
            fs.write("", 1);
        }
    }
}
