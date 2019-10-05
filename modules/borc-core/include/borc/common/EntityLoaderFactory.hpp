
#ifndef __BORC_COMMON_ENTITYLOADERFACTORY_HPP__
#define __BORC_COMMON_ENTITYLOADERFACTORY_HPP__

#include <boost/filesystem/path.hpp>
#include <memory>

namespace borc {
    class EntityLoader;
    class FileService;
    class EntityLoaderFactory {
    public:
        ~EntityLoaderFactory();

        std::unique_ptr<EntityLoader> createLoader(const boost::filesystem::path &packageFile, FileService &fileService);
    };
}

#endif
