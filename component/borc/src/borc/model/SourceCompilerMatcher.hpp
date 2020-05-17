
#ifndef __BOK_MODEL_SOURCEBUILDRULE_HPP__
#define __BOK_MODEL_SOURCEBUILDRULE_HPP__

#include <set>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Source;
    class Compiler;
    class SourceCompilerMatcher {
    public:
        explicit SourceCompilerMatcher(Compiler *compiler);

        ~SourceCompilerMatcher();

        bool match(const boost::filesystem::path &inputFileName) const {
            return true;
        }

        boost::filesystem::path computeOutputFile(const boost::filesystem::path &inputFileName) const {
            return inputFileName.string() + ".obj";
        };

    private:
        Compiler *compiler = nullptr;
        std::set<std::string> wildcards;
    };
}

#endif
