
#include <borc/services/BuildServiceImpl.hpp>

namespace borc {
    BuildServiceImpl::BuildServiceImpl(const boost::filesystem::path &outputFolder, Toolchain *toolchain) {
        this->outputFolder = outputFolder;
        this->toolchain = toolchain;
    }

    BuildServiceImpl::~BuildServiceImpl() {}

    void BuildServiceImpl::build(Package *package) {
        
    }
}
