
#ifndef __BORC_MODEL_BUILDPIPELINE_HPP__
#define __BORC_MODEL_BUILDPIPELINE_HPP__

#include <boost/filesystem/path.hpp>

namespace bok {
    class Source;

    class BuildPipeline {
    public:
        BuildPipeline();
        ~BuildPipeline();

        void build(const Source *source);
    };
}

#endif
