
#pragma once

#include "Linker.hpp"

namespace bok {
    class Linker_Mock : public Linker {
    public:
        LinkOutput generateLinkOutput(const LinkInput& input) override;
    };
}
