
#ifndef __BOK_MODEL_SOURCE_HPP__
#define __BOK_MODEL_SOURCE_HPP__

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Module;

    class Source {
    public:
        explicit Source(const Module *component, const boost::filesystem::path &filePath, const boost::filesystem::path &relativeFilePath);

        ~Source();

        const Module* getModule() const {
            return component;
        }

        boost::filesystem::path getFilePath() const {
            return filePath;
        }

        boost::filesystem::path getRelativeFilePath() const {
            return relativeFilePath;
        }

        std::time_t getModifiedTime() const;

    private:
        boost::filesystem::path filePath;
        boost::filesystem::path relativeFilePath;
        const Module *component = nullptr;
    };
}

#endif
