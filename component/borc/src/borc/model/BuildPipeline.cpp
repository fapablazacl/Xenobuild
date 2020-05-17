
#include <borc/model/BuildPipeline.hpp>

#include <iostream>
#include <borc/model/Source.hpp>

namespace bok {
    BuildPipeline::BuildPipeline() {
        
    }

    BuildPipeline::~BuildPipeline() {}

    void BuildPipeline::build(const Source *source) {
        std::cout << "Don't know how to build " << source->getFilePath().filename().string() << std::endl;
    }
}
