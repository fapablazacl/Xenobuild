
#ifndef __BORC_MODEL_SOURCEBUILDRULE_HPP__
#define __BORC_MODEL_SOURCEBUILDRULE_HPP__

#include <boost/filesystem/path.hpp>

namespace borc {
    class SourceBuildRule {
    public:
        bool match(const boost::filesystem::path &inputFileName) const {
            return true;
        }

        boost::filesystem::path computeOutputFile(const boost::filesystem::path &inputFileName) const {
            return inputFileName.string() + ".obj";
        };

    private:
        
    };
}

#endif
