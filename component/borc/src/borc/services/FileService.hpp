
#ifndef __BORC_SERVICES_FILESERVICE_HPP__
#define __BORC_SERVICES_FILESERVICE_HPP__

#include <string>

namespace bok {
    class FileService {
    public:
        virtual ~FileService();

        virtual std::string load(const std::string &filePath) const = 0;

        virtual void save(const std::string &filePath, const std::string &content) const = 0;

        virtual void touch(const std::string &filePath) const = 0;
    };
}

#endif
