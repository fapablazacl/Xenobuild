
#ifndef __BORC_FILEUTIL_HPP__
#define __BORC_FILEUTIL_HPP__

#include <string>

namespace borc {
    class FileUtil {
    private:
        FileUtil() = delete;
        ~FileUtil() = delete;

    public:
        static std::string load(const std::string &filename);

        static void save(const std::string &filename, const std::string &content);
    };
}

#endif
