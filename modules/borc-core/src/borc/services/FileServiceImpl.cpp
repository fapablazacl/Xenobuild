
#include <borc/services/FileServiceImpl.hpp>

#include <fstream>

namespace borc {
    FileServiceImpl::~FileServiceImpl() {}


    std::string FileServiceImpl::load(const std::string &filePath) const {
        typedef std::istreambuf_iterator<char> fstream_iterator;

        std::fstream fs;

        fs.open(filePath.c_str(), std::ios_base::in);
        if (!fs.is_open()) {
            throw std::runtime_error("Couldn't open the file '" + filePath + "'.");
        }

        fs.seekg(0);

        std::string content;
        content.assign(fstream_iterator(fs), fstream_iterator());

        return content;
    }


    void FileServiceImpl::save(const std::string &filePath, const std::string &content) const {
        std::fstream fs;

        fs.open(filePath.c_str(), std::ios_base::out);
        if (!fs.is_open()) {
            throw std::runtime_error("The file '" + filePath + "' could't be opened");
        }

        if (content.size() > 0) {
            fs.write(content.c_str(), content.size());
        } else {
            fs.write("", 1);
        }
    }


    void FileServiceImpl::touch(const std::string &filePath) const {
        std::ofstream os;
        os.open(filePath.c_str(), std::ios_base::out);
        os.close();
    }
}
