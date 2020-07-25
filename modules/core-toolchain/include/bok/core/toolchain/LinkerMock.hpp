
#pragma once

#include "Linker.hpp"

namespace bok {
    class LinkerMock : public Linker {
    public:
        LinkOutput generateLinkOutput(const LinkInput& input) override;
    };
}
