
#ifndef __BOK_MODEL_BUILDPIPELINE_HPP__
#define __BOK_MODEL_BUILDPIPELINE_HPP__

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
