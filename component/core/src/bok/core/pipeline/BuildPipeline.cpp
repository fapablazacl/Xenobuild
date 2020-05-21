
#include <bok/core/pipeline/BuildPipeline.hpp>

#include <iostream>
#include <bok/core/package/Source.hpp>

namespace bok {
    BuildPipeline::BuildPipeline() {}

    BuildPipeline::~BuildPipeline() {}

    void BuildPipeline::build(const Source *source) {
        std::cout << "Don't know how to build " << source->getFilePath().filename().string() << std::endl;
    }
}
