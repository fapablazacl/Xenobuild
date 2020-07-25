
#ifndef __BOK_SERVICES_FILESERVICEIMPL_HPP__
#define __BOK_SERVICES_FILESERVICEIMPL_HPP__

#include <string>

#include "FileService.hpp"

namespace bok {
    class FileService_FS : public FileService {
    public:
        virtual ~FileService_FS();

        virtual std::string load(const std::string &filePath) const override;

        virtual void save(const std::string &filePath, const std::string &content) const override;

        virtual void touch(const std::string &filePath) const override;
    };
}

#endif
