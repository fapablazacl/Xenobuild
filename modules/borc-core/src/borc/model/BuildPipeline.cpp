
#include <borc/model/BuildPipeline.hpp>

#include <iostream>

namespace borc {
    BuildPipeline::BuildPipeline() {}

    BuildPipeline::~BuildPipeline() {}

    void BuildPipeline::build(const boost::filesystem::path &filePath) {
        std::cout << "Don't know how to build " << filePath.filename().string() << std::endl;
    }
}
